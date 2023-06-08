// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Weekday.generated.h"

/// <summary>
/// The Weekday enumeration has a value corresponding to each day of the week.
/// </summary>
UENUM()
enum class UWeekday : uint8
{
	Monday = 0,
	Tuesday = 1,
	Wednesday = 2,
	Thursday = 3,
	Friday = 4,
	Saturday = 5,
	Sunday = 6
};