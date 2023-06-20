// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Hobby.generated.h"

/// <summary>
/// The Hobby enum has a value corresponding to each potential hobby an NPC can have.
/// </summary>
/*UENUM(BlueprintType)
enum class UHobby : uint8
{
	Exercise,
	Gymnastics,
	Jogging,
	Dancing,
	Swimming,
	Piano,
	Guitar,
	Gaming
};

*/
UCLASS()
class NPCVILLAGERS_API AHobby : public AActor
{
	GENERATED_BODY()

public:
	FString Name;

	int MinHobbyMinutes = 20;
	int MaxHobbyMinutes = 50;

	AActor* AssociatedObject;
//	UAnim* AssociatedAnimation;
};