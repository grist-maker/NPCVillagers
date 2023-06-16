// Fill out your copyright notice in the Description page of Project Settings.
#include "DialogBank.h"
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

FString UDialogBank::SelectMoodResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		Npc->Talk();
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return(OverrideCheck);
		}
		Npc->PlayerAffinity += Npc->TalkAffinity;

		int Response = 0;
		switch (Npc->Mood)
		{
		case UMood::Happy:
			Response = FMath::RandRange(0, HappyMood.Num() + NpcJob->HappyMood.Num() - 2);
			Npc->Happiness += Npc->MoodGain;
			if (Response >= HappyMood.Num())
			{
				return NpcJob->HappyMood[Response - HappyMood.Num()];
			}
			return HappyMood[Response];
		default:
			Npc->Excitement += Npc->MoodGain;
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

FString UDialogBank::SelectEnergyResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		Npc->Talk();
		int Response = 0;
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		Npc->PlayerAffinity += Npc->TalkAffinity;
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

FString UDialogBank::SelectRelationshipResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		Npc->Talk();
		int Response = 0;
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));

		FString OverrideCheck = CheckForOverride(Npc);
		if (OverrideCheck != "")
		{
			return OverrideCheck;
		}
		if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
		{
			Npc->PlayerAffinity -= Npc->TalkAffinity;
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
			Npc->PlayerAffinity += Npc->TalkAffinity;
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
			Npc->PlayerAffinity += Npc->TalkAffinity;
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

FString UDialogBank::SelectCoworkerResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());

	if (Npc != nullptr && Npc->Career != nullptr)
	{
		Npc->Talk();
	}
	return "";
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
			int Response = 0;

			if (Npc->Energy >= Npc->EnergyLevels[1])
			{
				Response = FMath::RandRange(0, HighEnergy.Num() + NpcJob->HighEnergy.Num() - 2);
				if (Response >= HighEnergy.Num())
				{
					ChosenResponses.Add(NpcJob->HighEnergy[Response - HighEnergy.Num()]);
				}
				else
				{
					ChosenResponses.Add(HighEnergy[Response]);
				}
			}
			else
			{
				Response = FMath::RandRange(0, MidEnergy.Num() + NpcJob->MidEnergy.Num() - 2);
				if (Response >= MidEnergy.Num())
				{
					ChosenResponses.Add(NpcJob->MidEnergy[Response - MidEnergy.Num()]);
				}
				else
				{
					ChosenResponses.Add(MidEnergy[Response]);
				}
			}

			if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				Response = FMath::RandRange(0, LowAffinity.Num() + NpcJob->LowAffinity.Num() - 2);
				if (Response >= LowAffinity.Num())
				{
					ChosenResponses.Add(NpcJob->LowAffinity[Response - LowAffinity.Num()]);
				}
				else
				{
					ChosenResponses.Add(LowAffinity[Response]);
				}
			}
			else if (Npc->PlayerAffinity >= Npc->AffinityLevels[1])
			{
				Response = FMath::RandRange(0, HighAffinity.Num() + NpcJob->HighAffinity.Num() - 2);
				if (Response >= HighAffinity.Num())
				{
					ChosenResponses.Add(NpcJob->HighAffinity[Response - HighAffinity.Num()]);
				}
				else
				{
					ChosenResponses.Add(HighAffinity[Response]);
				}
			}
			else
			{
				Response = FMath::RandRange(0, MidAffinity.Num() + NpcJob->MidAffinity.Num() - 2);
				if (Response >= MidAffinity.Num())
				{
					ChosenResponses.Add(NpcJob->MidAffinity[Response - MidAffinity.Num()]);
				}
				else
				{
					ChosenResponses.Add(MidAffinity[Response]);
				}
			}

			switch (Npc->Mood)
			{
			case UMood::Happy:
				Response = FMath::RandRange(0, HappyMood.Num() + NpcJob->HappyMood.Num() - 2);
				if (Response >= HappyMood.Num())
				{
					ChosenResponses.Add(NpcJob->HappyMood[Response - HappyMood.Num()]);
				}
				else
				{
					ChosenResponses.Add(HappyMood[Response]);
				}
				break;
			default:
				Response = FMath::RandRange(0, ExcitedMood.Num() + NpcJob->ExcitedMood.Num() - 2);
				if (Response >= ExcitedMood.Num())
				{
					ChosenResponses.Add(NpcJob->ExcitedMood[Response - ExcitedMood.Num()]);
				}
				else
				{
					ChosenResponses.Add(ExcitedMood[Response]);
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