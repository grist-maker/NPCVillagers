// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "TimeStamp.h"
#include "HobbyStation.h"
#include "Hobby.generated.h"

UCLASS()
class NPCVILLAGERS_API AHobby : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimestamp MinHobbyTime = { 0, 30 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimestamp MaxHobbyTime = { 2,30 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimestamp HobbyDelay{ 0,0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AHobbyStation* AssociatedObject;

	FTimestamp LastHobbyStartTime{ 0, 0 };
	FTimestamp ActiveHobbyEndTime = { 0, 0 };

	void DailyReset();
	bool CanInvoke(FTimestamp CurrentTime);
};