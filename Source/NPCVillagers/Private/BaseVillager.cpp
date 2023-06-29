// Fill out your copyright notice in the Description page of Project Settings.
#include "..\Public\BaseVillager.h"
#include "GameFramework/Pawn.h"
#include "PlayerVillager.h"
#include "GameFramework/Controller.h"

// Sets default values
ABaseVillager::ABaseVillager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseVillager::BeginPlay()
{
	Super::BeginPlay();
	NPCController = GetController();
	NPCAIController = Cast<AAIController>(NPCController);

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Responses = Cast<UDialogBank>(GetDefaultSubobjectByName("Dialog"));
	
	/*if (UseDialogueFile && Responses != nullptr)
	{
		FJsonSerializableArray Dialogue;
		TArray<FString>* CurrentSubBank = nullptr;
		if (FFileHelper::LoadANSITextFileToStrings(&FilePath, NULL, Dialogue))
		{
			for (int i = 0; i < Dialogue.Num(); i++)
			{
				FindSubBank(Dialogue[i], CurrentSubBank);
				if (CurrentSubBank != nullptr)
				{
					CurrentSubBank->Add(Dialogue[i]);
				}
			}
		}
	}*/
}

void ABaseVillager::FindSubBank(FString& StringValue, TArray<FString>* CurrentSubBank)
{
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
	if (StringValue.Equals("HighAffinity"))
	{
		CurrentSubBank = &Responses->HighAffinity;
	}
}

// Called every frame
void ABaseVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseVillager::UpdateMood()
{
	for (int i = 0; i < MoodValues.Num(); i++)
	{
		if (*MoodValues[i] < 0)
		{
			*MoodValues[i] = 0;
		}
		else if (*MoodValues[i] > 100)
		{
			*MoodValues[i] = 100;
		}
	}

	int j = 0;
	float LargestValue = 0;
	for (int i = 0; i < MoodValues.Num(); i++)
	{
		if (LargestValue < *MoodValues[i])
		{
			LargestValue = *MoodValues[i];
			j = i;
		}
	}
	Mood = static_cast<UMood>(j);
}

void ABaseVillager::UpdateAffinity()
{
	if (PlayerAffinity < 0)
	{
		PlayerAffinity = 0;
	}
	else if (PlayerAffinity > 100)
	{
		PlayerAffinity = 100;
	}
}

void ABaseVillager::UpdateEnergy(bool UpdateLevels)
{
	if (UpdateLevels)
	{
		Energy += *EnergyChange[static_cast<uint8>(State)];
		if (State != UState::Relaxing && State != UState::Unconscious && State!= UState::Asleep && (Mood == UMood::Angry || Mood == UMood::Sad || Mood == UMood::Scared))
		{
			Energy -= NegativeMoodEnergyDrain;
		}
	}

	if (Energy < 0)
	{
		Energy = 0;
	}
	else if (Energy > 100)
	{
		Energy = 100;
	}
}

void ABaseVillager::NegativeMoodHit(float MoodChange)
{
	auto NewValue = FMath::RandRange(0, 100);
	if (NewValue <= NegativeMoodPercent)
	{
		*MoodValues[static_cast<uint8>(NegativePreference)] += MoodChange;
	}
	//Negative moods use the integer values 1 2 and 3.
	else
	{
		int RandomMood = FMath::RandRange(1, 3);
		while (RandomMood == static_cast<uint8>(NegativePreference))
		{
			RandomMood = FMath::RandRange(1, 3);
		}
		*MoodValues[RandomMood] += MoodChange;
	}
	UpdateMood();
}

void ABaseVillager::PositiveMoodHit(float MoodChange)
{
	auto NewValue = FMath::RandRange(0, 100);
	if (NewValue <= PositiveMoodPercent)
	{
		*MoodValues[static_cast<uint8>(PositivePreference)]+=MoodChange;
	}
	else
	{
		if (PositivePreference == UMood::Happy)
		{
			Excitement += MoodChange;
		}
		else
		{
			Happiness += MoodChange;
		}
	}
	UpdateMood();
}


void ABaseVillager::Talk()
{
	PreviousState = State;
	State = UState::Talking;
	NPCAIController->StopMovement();
	SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
}

bool ABaseVillager::RecoverEnergy()
{
	if (Energy < 10)
	{
		NPCAIController->StopMovement();
		State = UState::Unconscious;
		return true;
	}
	else if (Energy < EnergyLevels[0] && !AtWork)
	{
		NPCAIController->StopMovement();
		State = UState::Relaxing;
		return true;
	}
	else if (Energy >= EnergyLevels[1] && (State == UState::Relaxing || State == UState::Unconscious))
	{
		if (AtWork)
		{
			State = UState::Working;
		}
		else
		{
			State = UState::Idle;
		}
		return false;
	}
	else if (State == UState::Relaxing || State == UState::Asleep || State == UState::Unconscious)
	{
		return true;
	}
	return false;
}

void ABaseVillager::RelaxingMoodImprovement()
{
	if (static_cast<uint8>(Mood) != 0)
	{
		*MoodValues[static_cast<uint8>(Mood)] -= MoodDrain/10;
	}
	else
	{
		NegativeMoodHit(-MoodDrain / 10);
	}
	PositiveMoodHit(MoodGain/10);
	UpdateMood();
}

void ABaseVillager::WorkingBehaviors()
{

}
void ABaseVillager::WalkingBehaviors()
{
	if (GetVelocity() == FVector{ 0, 0, 0 })
	{
		State = UState::Idle;
	}
	if (TargetHobby < Hobbies.Num())
	{
		auto& InvokedHobby = Hobbies[TargetHobby];
		if (IsOverlappingActor(Hobbies[TargetHobby]->AssociatedObject))
		{
			TravelingToHobby = false;
			CurrentHobby = InvokedHobby->Name;
			auto HobbyRange = InvokedHobby->MaxHobbyTime - InvokedHobby->MinHobbyTime;
			int ProposedHours = 0;
			int ProposedMinutes = 0;
			if (HobbyRange.Hour >= 1)
			{
				ProposedHours = FMath::RandRange(0, HobbyRange.Hour);
				if (ProposedHours == HobbyRange.Hour)
				{
					ProposedMinutes = FMath::RandRange(0, HobbyRange.Minute);
				}
				else
				{
					ProposedMinutes = FMath::RandRange(0, 59);
				}
			}
			else
			{
				ProposedMinutes = FMath::RandRange(0, HobbyRange.Minute);
			}
			FTimestamp FinalAmount = { ProposedHours, ProposedMinutes };
			FinalAmount = FinalAmount + InvokedHobby->MinHobbyTime;
			
			InvokedHobby->LastHobbyStartTime = CurrentTime;
			InvokedHobby->ActiveHobbyEndTime = FinalAmount + CurrentTime;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d:%d"), InvokedHobby->ActiveHobbyEndTime.Hour, InvokedHobby->ActiveHobbyEndTime.Minute));
			State = UState::Hobby;
		}
	}
}

void ABaseVillager::TalkingBehaviors()
{
	SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
}

void ABaseVillager::IdleBehaviors()
{
	int randomChance = FMath::RandRange(0, 100);
	if (CurrentTime.Minute % 10 == 0)
	{
		if (randomChance >= 80 && AttemptHobbySelection())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hobby selected"));
			State = UState::Walking;
			return;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hobby not selected"));
		}
	}

	if (GetVelocity() != FVector{ 0, 0, 0 })
	{
		State = UState::Walking;
	}
	else if (Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name)
	{
		SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
	}

	if (PlayerPawn != nullptr && NPCAIController != nullptr && !Commuting)
	{
		if (!GetWorldTimerManager().IsTimerActive(IdleTimer))
		{
			if (!(Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name))
			{
				GetWorldTimerManager().SetTimer(IdleTimer, this, &ABaseVillager::IdleLoafing, IdleSeconds, false);
			}
		}
	}
}
void ABaseVillager::HobbyBehaviors()
{
	if (TargetHobby < Hobbies.Num())
	{
		auto& InvokedHobby = Hobbies[TargetHobby];
		if (CurrentTime > InvokedHobby->ActiveHobbyEndTime)
		{
			State = UState::Idle;
		}
		else
		{
			PositiveMoodHit(MoodGain / 10);
			NegativeMoodHit(-MoodDrain / 10);
		}
	}
}
void ABaseVillager::RelaxingBehaviors()
{
	if (TargetHobby != Hobbies.Num())
	{
		Hobbies[TargetHobby]->ActiveHobbyEndTime = CurrentTime;
		TargetHobby = Hobbies.Num();
	}
}
void ABaseVillager::UnconsciousBehaviors()
{

}
void ABaseVillager::AsleepBehaviors()
{

}

void ABaseVillager::UpdateStatus()
{
	if (State != UState::Hobby && State != UState::Talking && CurrentHobby != "")
	{
		if (!TravelingToHobby)
		{
			TargetHobby = Hobbies.Num();
			CurrentHobby = "";
		}
		else
		{
			State = UState::Walking;
		}
	}
	if (State != UState::Talking && State != UState::Asleep && RecoverEnergy())
	{
		RelaxingMoodImprovement();
	}
	(this->*(Behaviors[static_cast<uint8>(State)]))();

	if(GetWorldTimerManager().IsTimerActive(IdleTimer) && State != UState::Idle)
	{
		GetWorldTimerManager().ClearTimer(IdleTimer);
	}
	UpdateEnergy(true);
}

void ABaseVillager::IdleLoafing()
{
	if (!(Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name))
	{
		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation Point;
		bool success = NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), IdleRadius, Point);
		if (success)
		{
			NPCAIController->MoveToLocation(Point);
			State = UState::Walking;
		}
	}
}

bool ABaseVillager::AttemptHobbySelection()
{
	auto HobbyIndex = FMath::RandRange(0, Hobbies.Num()-1);
	if (Hobbies[HobbyIndex] != nullptr && Hobbies[HobbyIndex]->AssociatedObject != nullptr  && Energy >40)
	{
		if (Hobbies[HobbyIndex]->CanInvoke(CurrentTime))
		{
			TravelingToHobby = true;
			TargetHobby = HobbyIndex;
			NPCAIController->MoveToLocation(Hobbies[HobbyIndex]->AssociatedObject->GetActorLocation());
			State = UState::Walking;
			return true;
		}
	}
	return false;
}

void ABaseVillager::EndDialog()
{
	if (State != UState::Unconscious)
	{
		State = PreviousState;

		if (State == UState::Working)
		{
			SetActorRotation((Career->Workstation->GetActorLocation() - GetActorLocation()).Rotation());
		}
	}
}

void ABaseVillager::CheckOnJob(const UWeekday& CurrentDay)
{
	if (State != UState::Talking && !DoneWork && Career != nullptr)
	{
		if (Career->CommuteTime.Hour <= CurrentTime.Hour && Career->CommuteTime.Minute <= CurrentTime.Minute && !Commuting && !AtWork && Career->Days.Contains(CurrentDay) && (Career->EndTime.Hour >= CurrentTime.Hour))
		{
			Commuting = true;
			State = UState::Walking;
			NPCAIController->MoveToActor(Career->Workstation, 0, true);
		}
		else if ( Career->EndTime.Hour == CurrentTime.Hour &&  Career->EndTime.Minute <= CurrentTime.Minute &&  AtWork)
		{
			AtWork = false;
			DoneWork = true;
			State = UState::Idle;
		}
		else if ( Commuting &&  IsOverlappingActor( Career->Workstation))
		{
			AtWork = true;
			Commuting = false;
			State = UState::Working;
			NPCAIController->StopMovement();
		}
		else if ( Commuting &&  State != UState::Talking &&  GetVelocity().GetAbsMax() < 1)
		{
			NPCAIController->MoveToActor( Career->Workstation, 0, true);
		}
	}
}

// Called to bind functionality to input
void ABaseVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}