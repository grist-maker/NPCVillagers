// Fill out your copyright notice in the Description page of Project Settings.
#include "..\Public\BaseVillager.h"
#include "GameFramework/Pawn.h"
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

	if (!Commuting && !AtWork && State != UState::Talking)
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

void ABaseVillager::UpdateStatus()
{
	if (PlayerPawn != nullptr && NPCAIController != nullptr && !Commuting && !AtWork && State != UState::Talking)
	{
		NPCAIController->MoveToActor(PlayerPawn, 100, true);
	}
}

void ABaseVillager::EndDialog()
{
	if (Commuting)
	{
		State = UState::Walking;
	}
	else if (AtWork)
	{
		State = UState::Working;
	}
	else
	{
		State = UState::Idle;
	}
}

// Called to bind functionality to input
void ABaseVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

