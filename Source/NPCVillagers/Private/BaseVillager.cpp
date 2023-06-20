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
}

// Called every frame
void ABaseVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ((Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name) && !AtWork)
	{
		SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
	}
	if (State == UState::Idle || State == UState::Walking)
	{
		if (GetVelocity() == FVector{ 0, 0, 0 })
		{
			State = UState::Idle;
		}
		else
		{
			State = UState::Walking;
		}
	}
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

void ABaseVillager::UpdateEnergy()
{
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
	else if (Energy >= EnergyLevels[1])
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
		*MoodValues[static_cast<uint8>(PositivePreference)] += MoodGain / 10;
		UpdateMood();
	}
}

void ABaseVillager::UpdateStatus()
{
	if (State != UState::Talking && State != UState::Asleep && RecoverEnergy())
	{
		RelaxingMoodImprovement();
	}

	if (PlayerPawn != nullptr && NPCAIController != nullptr && !Commuting && (State == UState::Idle))
	{
		if (!GetWorldTimerManager().IsTimerActive(IdleTimer))
		{
			if (!(Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name))
			{
				GetWorldTimerManager().SetTimer(IdleTimer, this, &ABaseVillager::IdleLoafing, IdleSeconds, false);
			}
		}
	}
	else if(GetWorldTimerManager().IsTimerActive(IdleTimer))
	{
		GetWorldTimerManager().ClearTimer(IdleTimer);
	}
	UpdateEnergy();
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
		}
	}
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

// Called to bind functionality to input
void ABaseVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}