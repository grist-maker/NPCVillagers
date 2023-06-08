// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeStamp.generated.h"

/// <summary>
/// The Timestamp struct is used to represent a point of time via an hour and minute portion, or an amount of time passed depending on use.
/// </summary>
USTRUCT(BlueprintType)
struct FTimestamp
{
public:
	GENERATED_BODY()
	/// <summary>
	/// The hour preserved by the timestamp.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Hour = 0;

	/// <summary>
	/// The minute preserved by the timestamp.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Minute = 0;
};