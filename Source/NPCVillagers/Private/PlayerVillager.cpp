// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerVillager.h"

// Sets default values
APlayerVillager::APlayerVillager()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Viewport = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	Viewport->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	Viewport->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	Viewport->bUsePawnControlRotation = true;
	Viewport->bUseControllerViewRotation_DEPRECATED;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerVillager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerVillager::MoveForward(float Amount)
{
	if (!(CurrentHobby != nullptr && CurrentHobby->PlayerPerforming))
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Amount);
	}
}

void APlayerVillager::MoveRight(float Amount)
{
	if (!(CurrentHobby != nullptr && CurrentHobby->PlayerPerforming))
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Amount);
	}
}

// Called to bind functionality to input
void APlayerVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerVillager::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerVillager::MoveRight);

	PlayerInputComponent->BindAxis("CameraX", this, &APlayerVillager::AddControllerYawInput);
	PlayerInputComponent->BindAxis("CameraY", this, &APlayerVillager::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerVillager::Jump);
}

