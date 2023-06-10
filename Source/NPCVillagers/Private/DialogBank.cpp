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
		return HurtResponse;
	}
	return "";
}

FString UDialogBank::SelectResponse()
{
	ABaseVillager* Npc = Cast<ABaseVillager>(GetOwner());
	if (Npc != nullptr && Npc->Career != nullptr)
	{
		UDialogBank* NpcJob = Cast<UDialogBank>(Npc->Career->GetComponentByClass(UDialogBank::StaticClass()));
		if (NpcJob != nullptr && Npc->NPCAIController != nullptr)
		{
			Npc->Talk();

			if (Npc->AtWork || Npc->Commuting)
			{
				Npc->PlayerAffinity -= Npc->TalkAffinity;

				if (FMath::RandRange(0, 1))
				{
					return BusyState[FMath::RandRange(0, BusyState.Num() - 1)];
				}
				else
				{
					return NpcJob->BusyState[FMath::RandRange(0, NpcJob->BusyState.Num() - 1)];
				}
			}

			if (Npc->Energy <= Npc->EnergyLevels[0])
			{
				ChosenResponses.Add(LowEnergy[FMath::RandRange(0, LowEnergy.Num() - 1)]);
				ChosenResponses.Add(NpcJob->LowEnergy[FMath::RandRange(0, NpcJob->LowEnergy.Num() - 1)]);
			}
			else if (Npc->Energy >= Npc->EnergyLevels[1])
			{
				ChosenResponses.Add(HighEnergy[FMath::RandRange(0, HighEnergy.Num() - 1)]);
				ChosenResponses.Add(NpcJob->HighEnergy[FMath::RandRange(0, NpcJob->HighEnergy.Num() - 1)]);
			}
			else
			{
				ChosenResponses.Add(MidEnergy[FMath::RandRange(0, MidEnergy.Num() - 1)]);
				ChosenResponses.Add(NpcJob->MidEnergy[FMath::RandRange(0, NpcJob->MidEnergy.Num() - 1)]);
			}

			if (Npc->PlayerAffinity <= Npc->AffinityLevels[0])
			{
				ChosenResponses.Add(LowAffinity[FMath::RandRange(0, LowAffinity.Num() - 1)]);
				ChosenResponses.Add(NpcJob->LowAffinity[FMath::RandRange(0, NpcJob->LowAffinity.Num() - 1)]);
			}
			else if (Npc->PlayerAffinity >= Npc->AffinityLevels[1])
			{
				ChosenResponses.Add(HighAffinity[FMath::RandRange(0, HighAffinity.Num() - 1)]);
				ChosenResponses.Add(NpcJob->HighAffinity[FMath::RandRange(0, NpcJob->HighAffinity.Num() - 1)]);
			}
			else
			{
				ChosenResponses.Add(MidAffinity[FMath::RandRange(0, MidAffinity.Num() - 1)]);
				ChosenResponses.Add(NpcJob->MidAffinity[FMath::RandRange(0, NpcJob->MidAffinity.Num() - 1)]);
			}

			switch (Npc->Mood)
			{
			case UMood::Happy:
				ChosenResponses.Add(HappyMood[FMath::RandRange(0, HappyMood.Num() - 1)]);
				ChosenResponses.Add(NpcJob->HappyMood[FMath::RandRange(0, NpcJob->HappyMood.Num() - 1)]);
				break;
			case UMood::Sad:
				ChosenResponses.Add(SadMood[FMath::RandRange(0, SadMood.Num() - 1)]);
				ChosenResponses.Add(NpcJob->SadMood[FMath::RandRange(0, NpcJob->SadMood.Num() - 1)]);
				break;
			case UMood::Angry:
				ChosenResponses.Add(AngryMood[FMath::RandRange(0, AngryMood.Num() - 1)]);
				ChosenResponses.Add(NpcJob->AngryMood[FMath::RandRange(0, NpcJob->AngryMood.Num() - 1)]);
				break;
			case UMood::Scared:
				ChosenResponses.Add(ScaredMood[FMath::RandRange(0, ScaredMood.Num() - 1)]);
				ChosenResponses.Add(NpcJob->ScaredMood[FMath::RandRange(0, NpcJob->ScaredMood.Num() - 1)]);
				break;
			default:
				ChosenResponses.Add(ExcitedMood[FMath::RandRange(0, ExcitedMood.Num() - 1)]);
				ChosenResponses.Add(NpcJob->ExcitedMood[FMath::RandRange(0, NpcJob->ExcitedMood.Num() - 1)]);
				break;
			}

			FString FinalResponse = ChosenResponses[FMath::RandRange(0, ChosenResponses.Num() - 1)];
			ChosenResponses.Empty();
			Npc->PlayerAffinity += Npc->TalkAffinity;
			return FinalResponse;
		}
	}
	return "";
}