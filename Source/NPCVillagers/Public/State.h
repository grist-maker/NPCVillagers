// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "State.generated.h"

/// <summary>
/// The State enum represents all physical states an NPC may be in in the world
/// </summary>
UENUM(BlueprintType)
enum class UState : uint8
{
	Working,
	Walking,
	Talking,
	Idle,
	Hobby,
	Relaxing,
	Unconscious,
	Asleep
};
