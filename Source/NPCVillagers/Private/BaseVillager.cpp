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
/*	if (StringValue.Equals("HighAffinity"))
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
	}*/
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

void ABaseVillager::ConverseTransition()
{
	if (ConversationPartner != nullptr && CurrentIdleBehavior == 2)
	{
		if (IsOverlappingActor(ConversationPartner))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Conversation Time!"));
			if (static_cast<APlayerVillager*>(ConversationPartner) && !static_cast<APlayerVillager*>(ConversationPartner)->ConversationTarget)
			{
				Talk();
				static_cast<APlayerVillager*>(PlayerPawn)->ConversationTarget = true;
				PlayerConversationInitiated = true;
			//	auto ConverseOptions = IdleOptions[CurrentIdleBehavior];
		//		CurrentTime.GenerateRandomTime(ConverseOptions->MinTime, ConverseOptions->MaxTime, ConverseOptions->StartTime, ConverseOptions->LastTime);
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d:%d"), ConverseOptions->LastTime.Hour, ConverseOptions->LastTime.Minute));
			}
		}
	}
}

void ABaseVillager::HobbyTransition()
{
	if (TargetHobby >= Hobbies.Num())
	{
		return;
	}
	auto& InvokedHobby = Hobbies[TargetHobby];
	if (Hobbies[TargetHobby] != nullptr && Hobbies[TargetHobby]->AssociatedObject != nullptr && IsOverlappingActor(Hobbies[TargetHobby]->AssociatedObject))
	{
		Hobbies[TargetHobby]->AssociatedObject->AddHobbyPerformer(this);
		TravelingToHobby = false;
		CurrentHobby = InvokedHobby->Name;

		CurrentTime.GenerateRandomTime(InvokedHobby->MinHobbyTime, InvokedHobby->MaxHobbyTime, InvokedHobby->LastHobbyStartTime, InvokedHobby->ActiveHobbyEndTime);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d:%d"), InvokedHobby->ActiveHobbyEndTime.Hour, InvokedHobby->ActiveHobbyEndTime.Minute));
		State = UState::Hobby;
	}
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
		if (State == UState::Hobby && TargetHobby < Hobbies.Num())
		{
			Hobbies[TargetHobby]->AssociatedObject->RemoveHobbyPerformer(this);
		}
		NPCAIController->StopMovement();
		State = UState::Unconscious;
		return true;
	}
	else if (Energy < EnergyLevels[0] && !AtWork && !GoingToBed)
	{
		if (State == UState::Hobby && TargetHobby < Hobbies.Num())
		{
			Hobbies[TargetHobby]->AssociatedObject->RemoveHobbyPerformer(this);
		}

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
	TravelingToHobby = false;
	CurrentIdleBehavior = 4;
}

void ABaseVillager::WalkingBehaviors()
{
	if (GetVelocity() == FVector{ 0, 0, 0 })
	{
		State = UState::Idle;
	}

	if (Commuting)
	{
		TravelingToHobby = false;
		CurrentIdleBehavior = 4;
		return;
	}

	if (GoingToBed && Bed != nullptr)
	{
		NPCAIController->MoveToLocation(Bed->GetActorLocation());
		if (IsOverlappingActor(Bed))
		{
			State = UState::Asleep;
		}
	}
	else if (CurrentIdleBehavior == 1)
	{
		HobbyTransition();
	}
	else if (CurrentIdleBehavior == 2)
	{
		ConverseTransition();
	}
}

void ABaseVillager::TalkingBehaviors()
{
	SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
	if (CurrentIdleBehavior == 2)
	{
		if (ConversationPartner != nullptr)
		{
			if (static_cast<APlayerVillager*>(ConversationPartner))
			{
				if (!CanTalk() || IdleOptions[CurrentIdleBehavior]->CurrentExecutions <= 0)
				{
					EndInitiatedDialog();
				}
			}
			else
			{
				if (IdleOptions[CurrentIdleBehavior]->LastTime < CurrentTime || !CanTalk())
				{
					EndInitiatedDialog();
				}
			}
		}
		else
		{
			EndDialog();
		}
	}
}
bool ABaseVillager::CanTalk()
{
	int BadMood = static_cast<uint8>(Mood);
	if (AtWork || Commuting || Energy <= EnergyLevels[0] || (BadMood != 0  && BadMood != 4))
	{
		return false;
	}
	return true;
}

void ABaseVillager::IdleBehaviors()
{
	CurrentIdleBehavior = 4;
	PlayerConversationInitiated = false;
	ConversationPartner = nullptr;
	TargetHobby = Hobbies.Num();

	if (!(Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name))
	{
		int randomChance = FMath::RandRange(0, 100);

		if (CurrentTime.Minute % 10 == 0)
		{
			if (randomChance <= IdleOptions[0]->Percent)
			{
				CurrentIdleBehavior = 0;
				IdleLoafing();
			}
			else if (randomChance <= IdleOptions[0]->Percent + IdleOptions[1]->Percent)
			{
				CurrentIdleBehavior = 1;
				if (AttemptHobbySelection())
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hobby selected"));
					State = UState::Walking;
					return;
				}
			}
			else
			{
				if (CanTalk())
				{
					CurrentIdleBehavior = 2;
					InitiateConversation();
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Conversation selected"));
				}
				else
				{
					CurrentIdleBehavior = 0;
					IdleLoafing();
				}
			}
		}

		if (GetVelocity() != FVector{ 0, 0, 0 })
		{
			State = UState::Walking;
		}
	}
	else if (Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name)
	{
		SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
	}
}

void ABaseVillager::DecrementConversations()
{
	IdleOptions[2]->CurrentExecutions -= 1;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), IdleOptions[2]->CurrentExecutions));
}

void ABaseVillager::InitiateConversation()
{
	ConversationPartner = ChoosePartner();
	if (ConversationPartner != nullptr)
	{
		NPCAIController->MoveToActor(ConversationPartner, 60, true);
		State = UState::Walking;
	}
}

AActor* ABaseVillager::ChoosePartner()
{
	if (!static_cast<APlayerVillager*>(PlayerPawn)->ConversationTarget)
	{
		auto NumberOfTimes = FMath::RandRange(IdleOptions[2]->MinExecutions, IdleOptions[2]->MaxExecutions);
		IdleOptions[2]->CurrentExecutions = NumberOfTimes;
		return PlayerPawn;
	}
	return nullptr;
}

void ABaseVillager::HobbyBehaviors()
{
	if (TargetHobby < Hobbies.Num())
	{
		auto& InvokedHobby = Hobbies[TargetHobby];
		if (CurrentTime.BetweenTwoTimes(InvokedHobby->LastHobbyStartTime, InvokedHobby->ActiveHobbyEndTime))
		{
			PositiveMoodHit(MoodGain / 10);
			NegativeMoodHit(-MoodDrain / 10);
			if (InvokedHobby->AssociatedObject->PlayerPerforming)
			{
				PlayerAffinity += HobbyAffinity;
			}
		}
		else
		{
			Hobbies[TargetHobby]->AssociatedObject->RemoveHobbyPerformer(this);
			State = UState::Idle;
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
	if (!CheckToSleep())
	{
		State = UState::Idle;
	}
}

void ABaseVillager::EndInitiatedDialog()
{
	CurrentIdleBehavior = 4;
	TargetHobby = Hobbies.Num();

	if(static_cast<APlayerVillager*>(ConversationPartner) && PlayerConversationInitiated)
	{
		static_cast<APlayerVillager*>(PlayerPawn)->InteractedNPC = nullptr;
		static_cast<APlayerVillager*>(PlayerPawn)->ConversationTarget = false;
		static_cast<APlayerVillager*>(PlayerPawn)->ConversationCleanup = true;
	}

	if (!Commuting && !AtWork && State != UState::Relaxing)
	{
		State = UState::Idle;
	}
	else if (Commuting)
	{
		State = UState::Walking;
	}
	else if (AtWork)
	{
		State = UState::Working;
		SetActorRotation((Career->Workstation->GetActorLocation() - GetActorLocation()).Rotation());
	}
	else if (Energy < EnergyLevels[0])
	{
		State = UState::Relaxing;
	}
	return;
}

void ABaseVillager::UpdateStatus()
{
	if (GetWorldTimerManager().IsTimerActive(IdleTimer) && State != UState::Idle)
	{
		GetWorldTimerManager().ClearTimer(IdleTimer);
	}
	CheckToSleep();
	if (GoingToBed)
	{
		TravelingToHobby = false;
		if (State != UState::Asleep && State != UState::Unconscious && State != UState::Talking)
		{
			NPCAIController->MoveToLocation(Bed->GetActorLocation());
			State = UState::Walking;
			if (IsOverlappingActor(Bed))
			{
				State = UState::Asleep;
			}
		}
	}
	if (State != UState::Hobby && State != UState::Talking && CurrentHobby != "")
	{
		if (!TravelingToHobby || Commuting || AtWork)
		{
			TargetHobby = Hobbies.Num();
			CurrentHobby = "";
		}
		else
		{
			State = UState::Walking;
		}
	}
	if (State != UState::Talking && RecoverEnergy())
	{
		RelaxingMoodImprovement();
	}
	(this->*(Behaviors[static_cast<uint8>(State)]))();
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

bool ABaseVillager::CheckToSleep()
{
	if (Career != nullptr)
	{
		GoingToBed = CurrentTime.BetweenTwoTimes(Career->SleepTime, Career->WakeTime);
		return GoingToBed;
	}
	return false;
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
	if (State != UState::Unconscious && State != UState::Asleep)
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
	if (!DoneWork && Career != nullptr)
	{
		if (State != UState::Talking || CurrentIdleBehavior < 3)
		{
			if (CurrentTime.BetweenTwoTimes(Career->CommuteTime, Career->EndTime) && !Commuting && !AtWork && Career->Days.Contains(CurrentDay))
			{
				Commuting = true;
				NPCAIController->MoveToActor(Career->Workstation, 0, true);
				if (State == UState::Talking)
				{
					EndInitiatedDialog();
				}
				State = UState::Walking;
			}
			else if (!CurrentTime.BetweenTwoTimes(Career->CommuteTime, Career->EndTime) && AtWork)
			{
				AtWork = false;
				DoneWork = true;
				State = UState::Idle;
			}
			else if (Commuting && IsOverlappingActor(Career->Workstation))
			{
				AtWork = true;
				Commuting = false;
				State = UState::Working;
				NPCAIController->StopMovement();
			}
			else if (Commuting && State != UState::Talking && GetVelocity().GetAbsMax() < 1)
			{
				NPCAIController->MoveToActor(Career->Workstation, 0, true);
			}
		}
	}
}

// Called to bind functionality to input
void ABaseVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}