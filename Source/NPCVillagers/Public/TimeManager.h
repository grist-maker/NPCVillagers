// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "Weekday.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerVillager.h"
#include "TimeStamp.h"
#include "BaseVillager.h"
#include "TimeManager.generated.h"

UCLASS()
class NPCVILLAGERS_API ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	APawn* Player;

	/// <summary>
	/// Represents the light acting as the sun in the world.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ADirectionalLight* Sun;

	/// <summary>
	/// Represents the current time within the world.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp CurrentTime = {0,0};

	/// <summary>
	/// Represents the current day in the world.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWeekday CurrentDay = UWeekday::Monday;

	/// <summary>
	/// These keep track of the randomized mood updates occuring 3 times a day.
	/// </summary>
	bool MorningMoodUpdate = false;

	bool NoonMoodUpdate = false;

	bool EveningMoodUpdate = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp MorningMoodTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp NoonMoodTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp EveningMoodTime;

	FTimestamp NextMoodTime = MorningMoodTime;
	/// <summary>
	/// Used to periodically update the game-world time based on the passage of fixed intervals of real-world time.
	/// </summary>
	FTimerHandle SecondCounter;

	/// <summary>
	/// An array of all of the Villagers in a scene, allowing the time manager to periodically update them with new data.
	/// </summary>
	TArray<AActor*> Villagers;
	/// <summary>
	/// A function that allows the Time Manager to adjust the energy levels of each villager with each in-game minute.
	/// </summary>
	void EnergyUpdate();
	/// <summary>
	/// A function that triggers job schedule based behavior for each NPC based on world time.
	/// </summary>
	void JobUpdate();

	TArray<ABaseVillager*> SoftwareEngineers;
	TArray<ABaseVillager*> Gardeners;
	TArray<ABaseVillager*> Fishers;
	TArray<ABaseVillager*> Merchants;

	TArray<TArray<ABaseVillager*>*> Coworkers
	{
		&SoftwareEngineers,
		&Gardeners,
		&Fishers,
		&Merchants
	};

private:
	/// <summary>
	/// Updates the energy and Job status of each villager in the scene.
	/// </summary>
	void UpdateVillagers();
	/// <summary>
	/// Progresses time in the scene by the amount specified by the Progress timestamp argument. This specifies the number of hours and minutes that the scene must be progressed by.
	/// </summary>
	/// <param name="Progress">A timestamp representing the hours and minutes to progress the scene clock by.</param>
	void ProgressTime(FTimestamp& Progress);
	/// <summary>
	/// This function updates the lighting on the "Sun" in the scene. It is done by a certain amount every hour that passes.
	/// </summary>
	/// <param name="Lux">The Lux setting for the Sun, effectively its intensity, specified as a floating point value.</param>
	void UpdateLighting(float Lux);

	/// <summary>
	/// A basic function called when the TimeManager begins operation, only called once.
	/// </summary>
	void BeginPlay();
	/// <summary>
	/// Progresses time by one minute, called over a fixed period.
	/// </summary>
	void NewMinute();

	void CheckForMoodUpdate();
};
