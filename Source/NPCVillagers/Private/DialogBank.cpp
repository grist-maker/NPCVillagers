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
			if (Npc->LikedGifts.Contains(GiftName))
			{
				Npc->PlayerAffinity += (Npc->LikedGiftAffinity);
				if (FMath::RandRange(0, 1))
				{
					Reaction = LikedGifts[FMath::RandRange(0, LikedGifts.Num() - 1)];
				}
				else
				{
					Reaction = NpcJob->LikedGifts[FMath::RandRange(0, NpcJob->LikedGifts.Num() - 1)];
				}
				Npc->PositiveMoodHit(Npc->MoodGain * 2);
				Npc->NegativeMoodHit(-Npc->MoodGain * 2);
			}
			else if (Npc->DislikedGifts.Contains(GiftName))
			{
				Npc->PlayerAffinity -= (Npc->DislikedGiftAffinity);
				if (FMath::RandRange(0, 1))
				{
					Reaction = DislikedGifts[FMath::RandRange(0, DislikedGifts.Num() - 1)];
				}
				else
				{
					Reaction = NpcJob->DislikedGifts[FMath::RandRange(0, NpcJob->DislikedGifts.Num() - 1)];
				}
				Npc->PositiveMoodHit(-Npc->MoodGain *2);
				Npc->NegativeMoodHit(Npc->MoodDrain * 2);
			}
			else
			{
				Npc->PlayerAffinity += (Npc->BaseGiftAffinity);
				if (FMath::RandRange(0, 1))
				{
					Reaction = MiddleGifts[FMath::RandRange(0, MiddleGifts.Num() - 1)];
				}
				else
				{
					Reaction = NpcJob->MiddleGifts[FMath::RandRange(0, NpcJob->MiddleGifts.Num() - 1)];
				}
				Npc->PositiveMoodHit(Npc->MoodGain);
				Npc->NegativeMoodHit(-Npc->MoodGain);
			}
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
		if (FMath::RandRange(0, 1))
		{
			HurtResponse = HurtReactions[FMath::RandRange(0, HurtReactions.Num() - 1)];
		}
		else
		{
			HurtResponse = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()))->HurtReactions[FMath::RandRange(0, Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()))->HurtReactions.Num() - 1)];
		}
		Npc->NegativeMoodHit(10);
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
			return NpcJob->BusyState[FMath::RandRange(0, NpcJob->BusyState.Num() - 1)];
		}
	}

	int Response = 0;
	if (Npc->Energy <= Npc->EnergyLevels[0])
	{
		Npc->PlayerAffinity -= Npc->TalkAffinity;
		Response = FMath::RandRange(0, LowEnergy.Num() + NpcJob->LowEnergy.Num() - 2);
		if (Response >= LowEnergy.Num())
		{
			return NpcJob->LowEnergy[Response - LowEnergy.Num()];
		}
		return LowEnergy[Response];
	}

	switch (Npc->Mood)
	{
		case UMood::Sad:
			Npc->PlayerAffinity -= Npc->TalkAffinity;
			Npc->Sadness += Npc->MoodDrain;
			Response = FMath::RandRange(0, SadMood.Num() + NpcJob->SadMood.Num() - 2);
			if (Response >= SadMood.Num())
			{
				return NpcJob->SadMood[Response - SadMood.Num()];
			}
			return SadMood[Response];
		case UMood::Angry:
			Npc->Anger += Npc->MoodDrain;
			Npc->PlayerAffinity -= Npc->TalkAffinity;
			Response = FMath::RandRange(0, AngryMood.Num() + NpcJob->AngryMood.Num() - 2);
			if (Response >= AngryMood.Num())
			{
				return NpcJob->AngryMood[Response - AngryMood.Num()];
			}
			return AngryMood[Response];
		case UMood::Scared:
			Npc->Fear += Npc->MoodDrain;
			Npc->PlayerAffinity -= Npc->TalkAffinity;
			Response = FMath::RandRange(0, ScaredMood.Num() + NpcJob->ScaredMood.Num() - 2);
			if (Response >= ScaredMood.Num())
			{
				return NpcJob->ScaredMood[Response - ScaredMood.Num()];
			}
			return ScaredMood[Response];
	}
	return "";
}

FString UDialogBank::SelectMoodResponse(bool AdjustStats)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		if (AdjustStats)
		{
			Npc->Talk();
		}
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return(OverrideCheck);
		}

		if (AdjustStats)
		{
			Npc->PlayerAffinity += Npc->TalkAffinity;
		}

		int Response = 0;
		switch (Npc->Mood)
		{
		case UMood::Happy:
			Response = FMath::RandRange(0, HappyMood.Num() + NpcJob->HappyMood.Num() - 2);

			if (AdjustStats)
			{
				Npc->Happiness += Npc->MoodGain;
			}

			if (Response >= HappyMood.Num())
			{
				return NpcJob->HappyMood[Response - HappyMood.Num()];
			}
			return HappyMood[Response];
		default:
			if (AdjustStats)
			{
				Npc->Excitement += Npc->MoodGain;
			}
			Response = FMath::RandRange(0, ExcitedMood.Num() + NpcJob->ExcitedMood.Num() - 2);
			if (Response >= ExcitedMood.Num())
			{
				return(NpcJob->ExcitedMood[Response - ExcitedMood.Num()]);
			}
			return(ExcitedMood[Response]);
		}
	}
	return "";
}

FString UDialogBank::SelectEnergyResponse(bool AdjustStats)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		if (AdjustStats)
		{
			Npc->Talk();
		}
		int Response = 0;
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		if (AdjustStats)
		{
			Npc->PlayerAffinity += Npc->TalkAffinity;
		}

		if (Npc->Energy >= Npc->EnergyLevels[1])
		{
			Response = FMath::RandRange(0, HighEnergy.Num() + NpcJob->HighEnergy.Num() - 2);
			if (Response >= HighEnergy.Num())
			{
				return(NpcJob->HighEnergy[Response - HighEnergy.Num()]);
			}
			else
			{
				return(HighEnergy[Response]);
			}
		}
		else
		{
			Response = FMath::RandRange(0, MidEnergy.Num() + NpcJob->MidEnergy.Num() - 2);
			if (Response >= MidEnergy.Num())
			{
				return(NpcJob->MidEnergy[Response - MidEnergy.Num()]);
			}
			else
			{
				return(MidEnergy[Response]);
			}
		}
	}
	return "";
}

FString UDialogBank::SelectRelationshipResponse(bool AdjustStats)
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		if (AdjustStats)
		{
			Npc->Talk();
		}
		int Response = 0;
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
		{
			if (AdjustStats)
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;
			}
			Response = FMath::RandRange(0, LowAffinity.Num() + NpcJob->LowAffinity.Num() - 2);
			if (Response >= LowAffinity.Num())
			{
				return(NpcJob->LowAffinity[Response - LowAffinity.Num()]);
			}
			else
			{
				return(LowAffinity[Response]);
			}
		}
		else if (Npc->PlayerAffinity >= Npc->AffinityLevels[1])
		{
			if (AdjustStats)
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
			}
			Response = FMath::RandRange(0, HighAffinity.Num() + NpcJob->HighAffinity.Num() - 2);
			if (Response >= HighAffinity.Num())
			{
				return(NpcJob->HighAffinity[Response - HighAffinity.Num()]);
			}
			else
			{
				return(HighAffinity[Response]);
			}
		}
		else
		{
			if (AdjustStats)
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
			}
			Response = FMath::RandRange(0, MidAffinity.Num() + NpcJob->MidAffinity.Num() - 2);
			if (Response >= MidAffinity.Num())
			{
				return(NpcJob->MidAffinity[Response - MidAffinity.Num()]);
			}
			else
			{
				return(MidAffinity[Response]);
			}
		}
	}
	return "";
}

FString UDialogBank::SelectWorkResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		Npc->Talk();
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));
		if (NpcJob != nullptr && Npc->NPCAIController != nullptr)
		{
			FString OverrideCheck = CheckForOverride(Npc);
			if (OverrideCheck != "")
			{
				return OverrideCheck;
			}

			if (Npc->Energy >= Npc->EnergyLevels[1])
			{
				ChosenResponses.Add(NpcJob->HighEnergy[FMath::RandRange(0, NpcJob->HighEnergy.Num() - 1)]);
			}
			else
			{
				ChosenResponses.Add(NpcJob->MidEnergy[FMath::RandRange(0, NpcJob->MidEnergy.Num() - 1)]);
			}

			if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				ChosenResponses.Add(NpcJob->LowAffinity[FMath::RandRange(0, NpcJob->LowAffinity.Num() - 1)]);
			}
			else if (Npc->PlayerAffinity >= Npc->AffinityLevels[1])
			{
				ChosenResponses.Add(NpcJob->HighAffinity[FMath::RandRange(0, NpcJob->HighAffinity.Num() - 1)]);
			}
			else
			{
				ChosenResponses.Add(NpcJob->MidAffinity[FMath::RandRange(0, NpcJob->MidAffinity.Num() - 1)]);
			}

			switch (Npc->Mood)
			{
			case UMood::Happy:
				ChosenResponses.Add(NpcJob->HappyMood[FMath::RandRange(0, NpcJob->HappyMood.Num() - 1)]);
				break;
			default:
				ChosenResponses.Add(NpcJob->ExcitedMood[FMath::RandRange(0, NpcJob->ExcitedMood.Num() - 1)]);
			}
			bool HitCoworker = false;
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
					HitCoworker = (*WorldManager->Coworkers[i])[j]->PlayerHits > 0;
					if (HitCoworker)
					{
						int Response = FMath::RandRange(0, NpcJob->HurtCoworker.Num() - 1);
						ChosenResponses.Add((*WorldManager->Coworkers[i])[j]->Name + (NpcJob->HurtCoworker[Response]));
					}
					else
					{
						int Response = FMath::RandRange(0, NpcJob->Coworker.Num() - 1);
						ChosenResponses.Add((*WorldManager->Coworkers[i])[j]->Name + (NpcJob->Coworker[Response]));
					}
				}
			}

			auto RandomSelection = FMath::RandRange(0, ChosenResponses.Num() - 1);
			if (RandomSelection == 1 && Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;
			}
			else
			{
				Npc->PlayerAffinity += Npc->TalkAffinity;
			}
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
		Npc->Talk();
		if (Npc != nullptr && Npc->NPCAIController != nullptr)
		{
			FString OverrideCheck = CheckForOverride(Npc);
			if (OverrideCheck != "")
			{
				return OverrideCheck;
			}

			if (Npc->Energy >= Npc->EnergyLevels[1])
			{
				ChosenResponses.Add(HighEnergy[FMath::RandRange(0, HighEnergy.Num() - 1)]);
			}
			else
			{
				ChosenResponses.Add(MidEnergy[FMath::RandRange(0, MidEnergy.Num() - 1)]);
			}

			if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				ChosenResponses.Add(LowAffinity[FMath::RandRange(0, LowAffinity.Num() - 1)]);
			}
			else if (Npc->PlayerAffinity >= Npc->AffinityLevels[1])
			{
				ChosenResponses.Add(HighAffinity[FMath::RandRange(0, HighAffinity.Num() - 1)]);
			}
			else
			{
				ChosenResponses.Add(MidAffinity[FMath::RandRange(0, MidAffinity.Num() - 1)]);
			}

			switch (Npc->Mood)
			{
			case UMood::Happy:
				ChosenResponses.Add(HappyMood[FMath::RandRange(0, HappyMood.Num() - 1)]);
				break;
			default:
				ChosenResponses.Add(ExcitedMood[FMath::RandRange(0, ExcitedMood.Num() - 1)]);
			}

			bool HitCoworker = false;
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
					HitCoworker = (*WorldManager->Coworkers[i])[j]->PlayerHits > 0;
					if (HitCoworker)
					{
						int Response = FMath::RandRange(0, HurtCoworker.Num() - 1);
						ChosenResponses.Add((*WorldManager->Coworkers[i])[j]->Name + (HurtCoworker[Response]));
					}
					else
					{
						int Response = FMath::RandRange(0, Coworker.Num() - 1);
						ChosenResponses.Add((*WorldManager->Coworkers[i])[j]->Name + (Coworker[Response]));
					}
				}
			}
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
			auto SelectedDialogue = ChosenResponses[RandomSelection];
			ChosenResponses.Empty();
			return SelectedDialogue;
		}
	}
	return "";
}

FString UDialogBank::SelectCoworkerResponse(bool& HitCoworker, bool AdjustStats)
{
	AdjustStats;
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());

	if (Npc != nullptr && Npc->Career != nullptr)
	{
		if (AdjustStats)
		{
			Npc->Talk();
		}
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
				HitCoworker = (*WorldManager->Coworkers[i])[j]->PlayerHits > 0;
				if (HitCoworker)
				{
					if (AdjustStats)
					{
						Npc->PlayerAffinity -= Npc->HurtAffinity;
					}
					int Response = FMath::RandRange(0, HurtCoworker.Num() + NpcJob->HurtCoworker.Num() - 2);

					if (Response >= HurtCoworker.Num())
					{
						return((*WorldManager->Coworkers[i])[j]->Name + (NpcJob->HurtCoworker[Response - HurtCoworker.Num()]));
					}
					else
					{
						return((*WorldManager->Coworkers[i])[j]->Name + (HurtCoworker[Response]));
					}
				}
				else
				{
					if (AdjustStats)
					{
						Npc->PlayerAffinity += Npc->TalkAffinity;
					}
					int Response = FMath::RandRange(0, Coworker.Num() + NpcJob->Coworker.Num() - 2);

					if (Response >= Coworker.Num())
					{
						return((*WorldManager->Coworkers[i])[j]->Name + (NpcJob->Coworker[Response - Coworker.Num()]));
					}
					else
					{
						return((*WorldManager->Coworkers[i])[j]->Name + (Coworker[Response]));
					}
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
		Npc->Talk();
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
				if (Npc->Mood == UMood::Happy)
				{
					Npc->Happiness += Npc->MoodGain;
				}
				else
				{
					Npc->Excitement += Npc->MoodGain;
				}
			}
			auto SelectedDialogue = ChosenResponses[RandomSelection];
			ChosenResponses.Empty();

			return SelectedDialogue;
		}
	}
	return "";
}