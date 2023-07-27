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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParameterFile")
		bool UseParameterFile = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParameterFile")
		FString ParameterFilePath;
		void LoadInParameters();
	bool FindParameter(FString& StringValue, int& PersonalityIndex);

	TArray<void*> Parameters
	{
		&MinHobbyTime,
		&MaxHobbyTime,
		&HobbyDelay,
		&Name
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};