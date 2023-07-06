// Fill out your copyright notice in the Description page of Project Settings.
#include "DialogBank.h"
#include "TimeManager.h"
#include "BaseVillager.h"

// Sets default values for this component's properties
UDialogBank::UDialogBank()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDialogBank::BeginPlay()
{
	WorldManager = static_cast<ATimeManager*>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimeManager::StaticClass()));
	Super::BeginPlay();
}


// Called every frame
void UDialogBank::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


FString UDialogBank::GiftReaction(FString GiftName)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr)
	{
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));
		if (NpcJob != nullptr && Npc->NPCAIController != nullptr)
		{
			Npc->Talk();

			FString Reaction = "";
			int GiftIndex = 0;
			if (Npc->LikedGifts.Contains(GiftName))
			{
				GiftIndex = 2;
				Npc->PlayerAffinity += (Npc->LikedGiftAffinity);
				Npc->PositiveMoodHit(Npc->MoodGain * 2);
				Npc->NegativeMoodHit(-Npc->MoodGain * 2);
			}
			else if (Npc->DislikedGifts.Contains(GiftName))
			{
				Npc->PlayerAffinity -= (Npc->DislikedGiftAffinity);
				Npc->PositiveMoodHit(-Npc->MoodGain *2);
				Npc->NegativeMoodHit(Npc->MoodDrain * 2);
			}
			else
			{
				GiftIndex = 1;
				Npc->PlayerAffinity += (Npc->BaseGiftAffinity);
				Npc->PositiveMoodHit(Npc->MoodGain);
				Npc->NegativeMoodHit(-Npc->MoodGain);
			}

			int Response = FMath::RandRange(0, GiftResponses[GiftIndex]->Num() + NpcJob->GiftResponses[GiftIndex]->Num() - 2);
			if (Response >= GiftResponses[GiftIndex]->Num())
			{
				Reaction = (*NpcJob->GiftResponses[GiftIndex])[Response - GiftResponses[GiftIndex]->Num()];
			}
			else
			{
				Reaction = (*GiftResponses[GiftIndex])[Response];
			}
			Npc->UpdateAffinity();
			return Reaction;
		}
	}
	return "";
}

FString UDialogBank::HurtReaction()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->NPCAIController != nullptr && Npc->Career != nullptr)
	{
		FString HurtResponse;
		Npc->Talk();
		Npc->PlayerAffinity -= (Npc->HurtAffinity);
		Npc->PlayerHits++;
		
		FVector LaunchDirection = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorForwardVector()*700;
		Npc->LaunchCharacter(LaunchDirection, false, false);
		if (FMath::RandRange(0, 1))
		{
			HurtResponse = HurtReactions[FMath::RandRange(0, HurtReactions.Num() - 1)];
		}
		else
		{
			HurtResponse = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()))->HurtReactions[FMath::RandRange(0, Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()))->HurtReactions.Num() - 1)];
		}
		Npc->NegativeMoodHit(10);
		Npc->PositiveMoodHit(-10);
		Npc->UpdateAffinity();
		return HurtResponse;
	}
	return "";
}

FString UDialogBank::CheckForOverride(ABaseVillager* Npc)
{
	UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

	if (NpcJob != nullptr && Npc->NPCAIController != nullptr)
	{
		if (Npc->AtWork || Npc->Commuting)
		{
			Npc->PlayerAffinity -= Npc->TalkAffinity;
			Npc->UpdateAffinity();
			return NpcJob->BusyState[FMath::RandRange(0, NpcJob->BusyState.Num() - 1)];
		}
	}

	int Response = 0;
	if (Npc->Energy <= Npc->EnergyLevels[0])
	{
		Npc->PlayerAffinity -= Npc->TalkAffinity;
		Npc->UpdateAffinity();
		Response = FMath::RandRange(0, LowEnergy.Num() + NpcJob->LowEnergy.Num() - 2);
		if (Response >= LowEnergy.Num())
		{
			return NpcJob->LowEnergy[Response - LowEnergy.Num()];
		}
		return LowEnergy[Response];
	}
	int BadMood = static_cast<uint8>(Npc->Mood);
	if (BadMood != 0 && BadMood != 4)
	{
		*Npc->MoodValues[BadMood] += Npc->MoodDrain;
		Npc->PlayerAffinity -= Npc->TalkAffinity;
		
		Npc->UpdateAffinity();
		Npc->UpdateMood();

		Response = FMath::RandRange(0, Moods[BadMood]->Num() + NpcJob->Moods[BadMood]->Num() - 2);
		if (Response >= Moods[BadMood]->Num())
		{
			return (*NpcJob->Moods[BadMood])[Response - Moods[BadMood]->Num()];
		}
		return (*Moods[BadMood])[Response];
	}
	return "";
}

FString UDialogBank::SelectMoodResponse(bool AdjustStats, bool IncludePersonality, bool IncludeJob)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
	//	if (AdjustStats)
		//{
			//Npc->Talk();
		//}
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return(OverrideCheck);
		}

		if (AdjustStats)
		{
			Npc->PlayerAffinity += Npc->TalkAffinity;
			Npc->UpdateAffinity();
		}

		int Response = 0;
		int CurrentMood = static_cast<uint8>(Npc->Mood);
		if (AdjustStats)
		{
			*Npc->MoodValues[CurrentMood] += Npc->MoodGain;
			Npc->UpdateMood();
		}

		Response = FMath::RandRange(0, Moods[CurrentMood]->Num() + NpcJob->Moods[CurrentMood]->Num() - 2);

		if (IncludePersonality && IncludeJob)
		{
			Response = FMath::RandRange(0, Moods[CurrentMood]->Num() + NpcJob->Moods[CurrentMood]->Num() - 2);
		}
		else if (IncludePersonality)
		{
			Response = FMath::RandRange(0, Moods[CurrentMood]->Num() - 1);
		}
		else if (IncludeJob)
		{
			Response = FMath::RandRange(0, NpcJob->Moods[CurrentMood]->Num() - 1);
		}

		if (IncludePersonality && IncludeJob)
		{
			if (Response >= Moods[CurrentMood]->Num())
			{
				return((*NpcJob->Moods[CurrentMood])[Response - Moods[CurrentMood]->Num()]);
			}
			return((*Moods[CurrentMood])[Response]);
		}
		else if (IncludePersonality)
		{
			return(*Moods[CurrentMood])[Response];
		}
		else if (IncludeJob)
		{
			return((*NpcJob->Moods[CurrentMood])[Response]);
		}
	}
	return "";
}

FString UDialogBank::SelectEnergyResponse(bool AdjustStats, bool IncludePersonality, bool IncludeJob)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
	//	if (AdjustStats)
		//{
	//		Npc->Talk();
		//}
		int Response = 0;
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);

		int EnergyAmount = 1;
		if (Npc->Energy <= Npc->EnergyLevels[0])
		{
			EnergyAmount = 0;
		}
		else if (Npc->Energy >= Npc->EnergyLevels[1])
		{
			EnergyAmount = 2;
		}

		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		if (AdjustStats)
		{
			Npc->PlayerAffinity += Npc->TalkAffinity;
			Npc->UpdateAffinity();
		}

		if (IncludePersonality && IncludeJob)
		{
			Response = FMath::RandRange(0, (*Energy[EnergyAmount]).Num() + (*NpcJob->Energy[EnergyAmount]).Num() - 2);
			if (Response >= (*Energy[EnergyAmount]).Num())
			{
				return((*NpcJob->Energy[EnergyAmount])[Response - (*Energy[EnergyAmount]).Num()]);
			}
			return((*Energy[EnergyAmount])[Response]);
		}
		else if (IncludePersonality)
		{
			Response = FMath::RandRange(0, (*Energy[EnergyAmount]).Num() - 1);
			return((*Energy[EnergyAmount])[Response]);
		}
		else if (IncludeJob)
		{
			Response = FMath::RandRange(0, (*NpcJob->Energy[EnergyAmount]).Num() - 1);
			return((*NpcJob->Energy[EnergyAmount])[Response]);
		}
	}
	return "";
}

FString UDialogBank::SelectRelationshipResponse(bool AdjustStats, bool IncludePersonality, bool IncludeJob)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
	//	if (AdjustStats)
		//{
	//		Npc->Talk();
		//}
		int Response = 0;
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}

		int AffinityAmount = 1;
		if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
		{
			AffinityAmount = 0;
			if (AdjustStats)
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;
				Npc->UpdateAffinity();
			}
		}
		else if (Npc->PlayerAffinity >= Npc->AffinityLevels[1])
		{
			AffinityAmount = 2;
			if (AdjustStats)
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
				Npc->UpdateAffinity();
			}
		}

		if (IncludePersonality && IncludeJob)
		{
			Response = FMath::RandRange(0, (*Affinity[AffinityAmount]).Num() + (*NpcJob->Affinity[AffinityAmount]).Num() - 2);
			if (Response >= (*Affinity[AffinityAmount]).Num())
			{
				return((*NpcJob->Affinity[AffinityAmount])[Response - (*Affinity[AffinityAmount]).Num()]);
			}
			return((*Affinity[AffinityAmount])[Response]);
		}
		else if(IncludePersonality)
		{
			Response = FMath::RandRange(0, (*Affinity[AffinityAmount]).Num() - 1);
			return((*Affinity[AffinityAmount])[Response]);
		}
		else if (IncludeJob)
		{
			Response = FMath::RandRange(0, (*NpcJob->Affinity[AffinityAmount]).Num() - 1);
			return((*NpcJob->Affinity[AffinityAmount])[Response]);
		}
	}
	return "";
}

FString UDialogBank::SelectWorkResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		//Npc->Talk();
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));
		if (NpcJob != nullptr && Npc->NPCAIController != nullptr)
		{
			FString OverrideCheck = CheckForOverride(Npc);
			if (OverrideCheck != "")
			{
				return OverrideCheck;
			}
			ChosenResponses.Add(SelectEnergyResponse(false, false, true));
			ChosenResponses.Add(SelectRelationshipResponse(false, false, true));
			ChosenResponses.Add(SelectMoodResponse(false, false, true));
	
			bool HitCoworker = false;
			ChosenResponses.Add(SelectCoworkerResponse(HitCoworker, false, false, true));

			auto RandomSelection = FMath::RandRange(0, ChosenResponses.Num() - 1);
			if (RandomSelection == 1 && Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;
			}
			else
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
			}

			if (RandomSelection == 2)
			{
				*Npc->MoodValues[static_cast<uint8>(Npc->Mood)] += Npc->MoodGain;
				Npc->UpdateMood();
			}

			Npc->UpdateAffinity();
			auto SelectedDialogue = ChosenResponses[RandomSelection];
			ChosenResponses.Empty();
			return SelectedDialogue;
		}
	}
	return "";
}

FString UDialogBank::SelectGenericResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr)
	{
	//	Npc->Talk();
		if (Npc != nullptr && Npc->NPCAIController != nullptr)
		{
			FString OverrideCheck = CheckForOverride(Npc);
			if (OverrideCheck != "")
			{
				return OverrideCheck;
			}
			ChosenResponses.Add(SelectEnergyResponse(false, true, false));
			ChosenResponses.Add(SelectRelationshipResponse(false, true, false));
			ChosenResponses.Add(SelectMoodResponse(false, true, false));
			bool HitCoworker = false;
			ChosenResponses.Add(SelectCoworkerResponse(HitCoworker, false, true, false));
			auto RandomSelection = FMath::RandRange(0, ChosenResponses.Num() - 1);
			
			if (RandomSelection == 1 && Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;
			}
			else if (RandomSelection == 3 && HitCoworker)
			{
				Npc->PlayerAffinity -= Npc->HurtAffinity;
			}
			else
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
			}

			if (RandomSelection == 2)
			{
				*Npc->MoodValues[static_cast<uint8>(Npc->Mood)] += Npc->MoodGain;
				Npc->UpdateMood();
			}

			Npc->UpdateAffinity();

			auto SelectedDialogue = ChosenResponses[RandomSelection];
			ChosenResponses.Empty();
			return SelectedDialogue;
		}
	}
	return "";
}

FString UDialogBank::SelectCoworkerResponse(bool& HitCoworker, bool AdjustStats, bool IncludePersonality, bool IncludeJob)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());

	if (Npc != nullptr && Npc->Career != nullptr)
	{
		//if (AdjustStats)
	//	{
		//	Npc->Talk();
		//}
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));
		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		if (WorldManager != nullptr)
		{
			int j = 0;
			int i = 0;

			for (i = 0; i < WorldManager->Coworkers.Num(); i++)
			{
				auto CoworkerArray = WorldManager->Coworkers[i];
				if (CoworkerArray != nullptr && CoworkerArray->Contains(Npc))
				{
					if (CoworkerArray->Num() > 1)
					{
						j = CoworkerArray->IndexOfByKey(Npc);
						int coworkerIndex = j;
						while (j == coworkerIndex)
						{
							j = FMath::RandRange(0, CoworkerArray->Num() - 1);
						}
						break;
					}
				}
			}

			if (i != WorldManager->Coworkers.Num())
			{
				int WasCoworkerHit = (*WorldManager->Coworkers[i])[j]->PlayerHits >0;

				if (AdjustStats)
				{
					if (WasCoworkerHit)
					{
						Npc->PlayerAffinity -= Npc->HurtAffinity;
					}
					else
					{
						Npc->PlayerAffinity += Npc->TalkAffinity;
					}
					Npc->UpdateAffinity();
				}
				int Response = 0;
			
				if (IncludePersonality && IncludeJob)
				{
					Response = FMath::RandRange(0, CoworkerResponses[WasCoworkerHit]->Num() + NpcJob->CoworkerResponses[WasCoworkerHit]->Num() - 2);
				}
				else if (IncludePersonality)
				{
					Response = FMath::RandRange(0, CoworkerResponses[WasCoworkerHit]->Num() - 1);
				}
				else if (IncludeJob)
				{
					Response = FMath::RandRange(0, NpcJob->CoworkerResponses[WasCoworkerHit]->Num() - 1);
				}
				if (IncludePersonality && IncludeJob)
				{
					if (Response >= CoworkerResponses[WasCoworkerHit]->Num())
					{
						return((*WorldManager->Coworkers[i])[j]->Name + ((* NpcJob->CoworkerResponses[WasCoworkerHit])[Response - CoworkerResponses[WasCoworkerHit]->Num()]));
					}
					else
					{
						return((*WorldManager->Coworkers[i])[j]->Name + ((*CoworkerResponses[WasCoworkerHit])[Response]));
					}
				}
				else if (IncludePersonality)
				{
					return((*WorldManager->Coworkers[i])[j]->Name + ((*CoworkerResponses[WasCoworkerHit])[Response]));
				}
				else if (IncludeJob)
				{
					return((*WorldManager->Coworkers[i])[j]->Name + ((*NpcJob->CoworkerResponses[WasCoworkerHit])[Response]));
				}
			}
		}
	}
	return "Coworkers? I don't have any of those.";
}

FString UDialogBank::SelectRandomResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		//Npc->Talk();
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));
		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		if (NpcJob != nullptr && Npc->NPCAIController != nullptr)
		{
			bool HitCoworker = false;
			ChosenResponses.Add(SelectEnergyResponse(false));
			ChosenResponses.Add(SelectRelationshipResponse(false));
			ChosenResponses.Add(SelectMoodResponse(false));
			ChosenResponses.Add(SelectCoworkerResponse(HitCoworker, false));

			auto RandomSelection = FMath::RandRange(0, ChosenResponses.Num() - 1);
			if (RandomSelection == 1 && Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;
			}
			else if (RandomSelection == 3 && HitCoworker)
			{
				Npc->PlayerAffinity -= Npc->HurtAffinity;
			}
			else
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
			}

			if (RandomSelection == 2)
			{
				(*Npc->MoodValues[static_cast<uint8>(Npc->Mood)]) += Npc->MoodGain;
				Npc->UpdateMood();
			}

			Npc->UpdateAffinity();
			auto SelectedDialogue = ChosenResponses[RandomSelection];
			ChosenResponses.Empty();

			return SelectedDialogue;
		}
	}
	return "";
}