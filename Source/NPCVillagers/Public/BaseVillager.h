// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Hobby.h"
#include "State.h"
#include "Mood.h"
#include <vector>
#include "Kismet/GameplayStatics.h"
#include <AIModule/Classes/AIController.h>
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "AIModule.h"
#include "Job.h"
#include "GameFramework/PlayerController.h"
#include "DialogBank.h"
#include "NavigationSystem.h"
#include "AbstractNavData.h"
#include "Misc/FileHelper.h"
#include "BaseVillager.generated.h"

UCLASS()
class NPCVILLAGERS_API ABaseVillager : public ACharacter
{
	GENERATED_BODY()

public:
	AController* NPCController;
	AAIController* NPCAIController;
	APawn* PlayerPawn;

	// Sets default values for this character's properties
	ABaseVillager();
	
	/// <summary>
	/// Unique category is intended to be unique for each villager. This includes things like name, age, and relevant
	/// data that are not determined by blocks, do not change, and can be used to identify a villager.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique")
	class UObject* Bed;

	/// <summary>
	/// Block category is used to specify the building blocks used to make up the NPC. As the name implies, the building
	/// blocks can be used to build up different NPCs. They may be unique per NPC, or reused between them.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<AHobby*> Hobbies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<FString>LikedGifts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<FString> DislikedGifts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	AJob* Career;

	UPROPERTY(VisibleAnywhere, Category = "Blocks")
	UDialogBank* Responses;

	/// <summary>
	/// Variables are values that are consistently being adjusted through actions and changes in the environment.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") //The physical state (what they're doing) of the NPC.
	UState State;

	UState PreviousState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") //The emotional state of the NPC.
	UMood Mood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Energy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") //The affinity the NPC has towards the player (0-100)
	float PlayerAffinity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") //How many times the player has hit the NPC.
	int PlayerHits;



	/// <summary>
	/// Mood variables are those that adjust the mood state over time. This includes the target of moods
	/// (what last changed their mood) and their mood levels on a scale from 0-100.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's happiness level.
	float Happiness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's anger level.
	float Anger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's fear level
	float Fear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's sadness level
	float Sadness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's excitement level
	float Excitement;

	TArray<float*> MoodValues
	{
		&Happiness,
		&Sadness,
		&Anger,
		&Fear,
		&Excitement
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's happiness target.
	class UObject* HappinessTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's anger target.
	UObject* AngerTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's fear target.
	UObject* FearTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's sadness target.
	UObject* SadnessTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's excitement target.
	UObject* ExcitementTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's usual mood when something positive happens.
	UMood PositivePreference = UMood::Happy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's usual mood when something negative happens.
	UMood NegativePreference = UMood::Sad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts") //Percent of the time the NPC will gain in their positive preferred mood when something good happens.
		float PositiveMoodPercent = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts") //Percent of the time the NPC will gain in their negative preferred mood when something bad happens.
		float NegativeMoodPercent = 50.0f;

	/// <summary>
	/// Gauge levels are used to let the user specify what this villager considers "high" or "low" levels for
	/// their meters. Anything between them is considered neutral, or mid-level. The first value is low, the second
	/// is high.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeLevels")
	TArray<float> AffinityLevels = {30, 70};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeLevels")
	TArray<float> EnergyLevels = { 30, 70 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeLevels")
	TArray<float> MoodLevels = { 30, 70 };

	/// <summary>
	/// Change Amounts specify how much an NPC's corresponding meter is adjusted when an action is performed.
	/// Users can tailor how much an NPC, for example, likes receiving a gift. These may be populated by personality
	/// overloads, or on an individual villager basis.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float HurtAffinity = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float LikedGiftAffinity = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float BaseGiftAffinity = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float DislikedGiftAffinity = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float TalkAffinity = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float HobbyAffinity = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float MoodGain = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float MoodDrain = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float RelaxEnergyGain = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float UnconsciousEnergyGain = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float SleepEnergyGain = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float JobEnergyDrain = -0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float BaseEnergyDrain = -0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float HobbyEnergyDrain =- 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts")
	float NegativeMoodEnergyDrain = -0.2f;

	/// <summary>
	/// Objective variables are used to dictate what the NPC is planning on doing next.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool Commuting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool AtWork = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool DoneWork = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void WorkingBehaviors();
	void WalkingBehaviors();
	void TalkingBehaviors();
	void IdleBehaviors();
	void HobbyBehaviors();
	void RelaxingBehaviors();
	void UnconsciousBehaviors();
	void AsleepBehaviors();
	
	typedef void (ABaseVillager::*StateBehaviors)(void);
	StateBehaviors Behaviors[8]
	{
		&ABaseVillager::WorkingBehaviors,
		&ABaseVillager::WalkingBehaviors,
		&ABaseVillager::TalkingBehaviors,
		&ABaseVillager::IdleBehaviors,
		&ABaseVillager::HobbyBehaviors,
		&ABaseVillager::RelaxingBehaviors,
		&ABaseVillager::UnconsciousBehaviors,
		&ABaseVillager::AsleepBehaviors
	};

	float* EnergyChange[8]
	{
		&JobEnergyDrain,
		&BaseEnergyDrain,
		&BaseEnergyDrain,
		&BaseEnergyDrain,
		&HobbyEnergyDrain,
		&RelaxEnergyGain,
		&UnconsciousEnergyGain,
		&SleepEnergyGain
	};

	void UpdateEnergy(bool UpdateLevels);
	/// <summary>
	/// This function updates the status of an NPC, adjusting their behavior based on current state and settings.
	/// </summary>
	void UpdateStatus();

	bool RecoverEnergy();

	void RelaxingMoodImprovement();

	void UpdateMood();

	void NegativeMoodHit(float MoodChange);

	void PositiveMoodHit(float MoodChange);

	void Talk();

	void IdleLoafing();

	void UpdateAffinity();

	bool AttemptHobbySelection();

	void CheckOnJob(const UWeekday& CurrentDay);

	FTimerHandle IdleTimer;

	FTimestamp CurrentTime;

	int TargetHobby = Hobbies.Num();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurrentHobby = "";

	float IdleSeconds = 4.0f;
	float IdleRadius = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueFile")
	bool UseDialogueFile = false;

	bool TravelingToHobby = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueFile")
	//TCHAR FilePath;


	void FindSubBank(FString& StringValue, TArray<FString>* CurrentSubBank);
	/// <summary>
	/// This function wraps up conversation with the NPC by returning them to their previous state.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void EndDialog();
};
