// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "DialogBank.generated.h"

class ABaseVillager;
class ATimeManager;

USTRUCT(BlueprintType)
struct FFSubBank
{
public:
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> Responses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Active;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NPCVILLAGERS_API UDialogBank : public UActorComponent
{
	GENERATED_BODY()

public:	

	ATimeManager* WorldManager;
	/// <summary>
	/// These are the Affinity sub banks. One is active at a time, corresponding to the current player-affinity level of the NPC and their meter settings.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> HighAffinity;
	UPROPERTY(EditAnywhere)
	TArray<FString> MidAffinity;
	UPROPERTY(EditAnywhere)
	TArray<FString> LowAffinity;

	TArray<TArray<FString>*> Affinity
	{
		&LowAffinity,
		&MidAffinity,
		&HighAffinity
	};

	/// <summary>
	/// These are the Energy sub banks. One is active at a time, corresponding to the current energy level of the NPC and their meter settings.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> HighEnergy;
	UPROPERTY(EditAnywhere)
	TArray<FString> MidEnergy;
	UPROPERTY(EditAnywhere)
	TArray<FString> LowEnergy;

	TArray<TArray<FString>*> Energy
	{
		&LowEnergy,
		&MidEnergy,
		&HighEnergy
	};

	/// <summary>
	/// These are the Mood sub banks. One is active at a time, corresponding to the current mood of the NPC.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> HappyMood;
	UPROPERTY(EditAnywhere)
	TArray<FString> SadMood;
	UPROPERTY(EditAnywhere)
	TArray<FString>	AngryMood;
	UPROPERTY(EditAnywhere)
	TArray<FString> ExcitedMood;
	UPROPERTY(EditAnywhere)
	TArray<FString> ScaredMood;

	TArray<TArray<FString>*> Moods
	{
		&HappyMood,
		&SadMood,
		&AngryMood,
		&ScaredMood,
		&ExcitedMood
	};

	/// <summary>
	/// /This is the Busy sub bank, activated when the player tries to converse with an NPC that is currently working.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> BusyState;

	/// <summary>
	/// This is the HurtReaction sub bank, activated when the player hits the NPC.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> HurtReactions;

	/// <summary>
	/// These are the Gift sub banks, with only one being active when the NPC receives a gift. Which one is activated is determined by the gift received and the gift's presence in the liked or disliked gift arrays.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> LikedGifts;
	UPROPERTY(EditAnywhere)
	TArray<FString> MiddleGifts;
	UPROPERTY(EditAnywhere)
	TArray<FString> DislikedGifts;
	TArray<TArray<FString>*> GiftResponses
	{
		&DislikedGifts,
		&MiddleGifts,
		&LikedGifts
	};

	/// <summary>
	/// This is the coworker sub-bank, used to provide default responses about their coworkers.
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<FString> Coworker;
	
	UPROPERTY(EditAnywhere)
	TArray<FString> HurtCoworker;

	TArray<TArray<FString>*> CoworkerResponses
	{
		&Coworker,
		&HurtCoworker
	};

	/// <summary>
	/// This is an array used to store potential responses as they are randomly chosen from sub-banks.
	/// </summary>
	TArray<FString> ChosenResponses;

	UDialogBank();

	/// <summary>
	/// The SelectResponse function governs the behavior of an NPC when the player chooses to talk to them.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FString SelectRandomResponse();

	UFUNCTION(BlueprintCallable)
	FString SelectMoodResponse(bool AdjustStats = true, bool IncludePersonality = true, bool IncludeJob = true);

	UFUNCTION(BlueprintCallable)
	FString SelectEnergyResponse(bool AdjustStats = true, bool IncludePersonality = true, bool IncludeJob = true);

	UFUNCTION(BlueprintCallable)
		FString SelectRelationshipResponse(bool AdjustStats = true, bool IncludePersonality = true, bool IncludeJob = true);

	UFUNCTION(BlueprintCallable)
	FString SelectGenericResponse();

	UFUNCTION(BlueprintCallable)
	FString SelectWorkResponse();
	
	UFUNCTION(BlueprintCallable)
	FString SelectCoworkerResponse(bool& HitCoworker, bool AdjustStats = true, bool IncludePersonality = true, bool IncludeJob = true);

	UFUNCTION(BlueprintCallable)
	FString CheckForOverride(ABaseVillager* Npc);
	/// <summary>
	/// The HurtReaction function governs the behavior of an NPC when the player chooses to hit them.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FString HurtReaction();
	/// <summary>
	/// The GiftReacion function governs the behavior of an NPC when the player gives them a gift.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FString GiftReaction(FString GiftName);

protected:
	/// <summary>
	/// This function is called only once: when the DialogBank begins operating in a scene.
	/// </summary>
	virtual void BeginPlay() override;

public:	
	/// <summary>
	/// This is called every frame for the DialogBank's operation.
	/// </summary>
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};