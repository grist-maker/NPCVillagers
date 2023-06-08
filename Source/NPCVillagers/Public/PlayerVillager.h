// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerVillager.generated.h"

UCLASS()
class NPCVILLAGERS_API APlayerVillager : public ACharacter
{
	GENERATED_BODY()

private:
	/// <summary>
	/// Allows forward and backward movement of the player character.
	/// </summary>
	UFUNCTION()
	void MoveForward(float Amount);

	/// <summary>
	/// Allows left and right movement of the player character
	/// </summary>
	UFUNCTION()
	void MoveRight(float Amount);

public:
	// Sets default values for this character's properties
	APlayerVillager();

	/// <summary>
	/// The camera component attached to the player in the scene.
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Viewport;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
