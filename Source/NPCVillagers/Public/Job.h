// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weekday.h"
#include "Components/ActorComponent.h"
#include "TimeStamp.h"
#include "Job.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NPCVILLAGERS_API AJob : public AActor
{
	GENERATED_BODY()

public:	
	/// <summary>
	/// The constructor for the Job actor, setting up any necessary dependencies for its functionality.
	/// </summary>
	AJob();

	/// <summary>
	/// The name of the Job that the NPC has.
	/// </summary>
	FString JobTitle;

	/// <summary>
	/// This is an array of Weekdays, representing the days that the job is active for the NPC that has it.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UWeekday> Days;

	/// <summary>
	/// This is the time that the Job begins for the NPC, the time that they are expected to be on the clock and working.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp StartTime;
	/// <summary>
	/// This is the time that the job ends for the NPC, the time that they are expected to be free to be off work.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp EndTime;
	/// <summary>
	/// This is the commute time for the NPC, the time they begin walking to their workstation.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp CommuteTime;
	/// <summary>
	/// This is the Workstation for the NPC, the place that they go to work.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Workstation;

protected:
	/// <summary>
	/// This function is called once, at the beginning of the Job's time in the scene.
	/// </summary>
	virtual void BeginPlay() override;
};
