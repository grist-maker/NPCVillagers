// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Mood.generated.h"

/// <summary>
/// The Mood enum represents all valid moods that an NPC may have.
/// </summary>
UENUM(BlueprintType)
enum class UMood : uint8
{
	Happy,
	Sad,
	Angry,
	Scared,
	Excited
};
 