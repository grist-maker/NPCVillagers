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
#include "SubBehavior.h"
#include "BaseVillager.generated.h"

UCLASS()
class NPCVILLAGERS_API ABaseVillager : public ACharacter
{
	GENERATED_BODY()

public:
	AController* NPCController; //The villager's basic controller component.
	AAIController* NPCAIController; //The villager's AI controller that governs much of their behavior.
	APawn* PlayerPawn; //A reference to the player.
	ATimeManager* WorldManager; //A reference to the world's manager, which tracks time as it passes.
	FTimestamp CurrentTime; // The current time within the world.
	ABaseVillager();

	/// <summary>
	/// Unique category is intended to be unique for each villager. This includes things like name, age, and relevant
	/// data that are not determined by blocks, do not change, and can be used to identify a villager.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique")
	FString Name; //The villager's name, a string that's used to identify them.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique")
	AActor* Bed; //A reference to the villager's bed, which they walk to and rest in when their bedtime is reached.


	/// <summary>
	/// Block category is used to specify the building blocks used to make up the NPC. As the name implies, the building
	/// blocks can be used to build up different NPCs. They may be unique per NPC, or reused between them.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<AHobby*> Hobbies; //An array of hobby object references. These are all the hobbies that the villager is able to invoke.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<FString>LikedGifts; //This array of strings has values that each correspond to the name of a gift the villager likes.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	TArray<FString> DislikedGifts; //This array of strings has values that each correspond to the name of a gift the villager dislikes.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocks")
	AJob* Career; //A reference to the villager's job. This object contains the relevant variables that allow the villager to go to work.

	UPROPERTY(VisibleAnywhere, Category = "Blocks")
	UDialogBank* Responses; // This is a pointer to the DialogBank component that corresponds to the villager's personality-specific dialog.


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's usual mood when something positive happens.
	UMood PositivePreference = UMood::Happy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoodVariables") //Represents the NPC's usual mood when something negative happens.
	UMood NegativePreference = UMood::Sad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts") //Percent of the time the NPC will gain in their positive preferred mood when something good happens.
	float PositiveMoodPercent = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeAmounts") //Percent of the time the NPC will gain in their negative preferred mood when something bad happens.
	float NegativeMoodPercent = 50.0f;

	void RelaxingMoodImprovement(); // This is the mood improvement triggered while the villager is relaxing.
	void UpdateMood(); // Updates the active mood, and adjusted mood values, of the NPC.
	void NegativeMoodHit(float MoodChange); // Gives a random negative mood the change as specified in the MoodChange variable.
	void PositiveMoodHit(float MoodChange); // Gives a random positive mood the change as specified in the MoodChange variable.


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

	float* EnergyChange[8] //This consolidates all the Energy values and indexes them based on the state value they correspond to.
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


	/// <summary>
	/// Objective variables are used to dictate what the NPC is planning on doing next.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool Commuting = false; // Indicates whether or not the villager is currently commuting to work.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool AtWork = false; // Indicates whether or not the villager is currently at work.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool DoneWork = false; //Indicates if the villager is done work for the day.
	
	bool TravelingToHobby = false; //Indicates whether the villager is currently traveling towards a hobby.
	bool GoingToBed = false; //Indicates whether the villager is in the process of going to bed.


	/// <summary>
	/// The Idle Options variables all relate specifically to Idle subbehaviors. These subbehaviors include Meander, Hobby, and Converse. Each subbehavior is transitioned to while the villager is aimless in the
	/// idle state.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleOptions")
	FSubbehavior MeanderBehavior{ USubbehaviors::Meander }; //Meander behavior customizable options
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleOptions")
	FSubbehavior HobbyBehavior{ USubbehaviors::Hobby }; //Hobby behavior customizable options
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleOptions")
	FSubbehavior ConverseBehavior{ USubbehaviors::Converse }; //Converse behavior customizable options

	FSubbehavior* IdleOptions[3] //An array containing references to each of the subbehaviors.
	{
		&MeanderBehavior,
		&HobbyBehavior,
		&ConverseBehavior
	};
	int CurrentIdleBehavior = 3; //The current sub-behavior being invoked by the villager, fully adjusted in code


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/// <summary>
	/// These two functions, HobbyTransition and ConverseTransition, invoke the beginning of the hobby and converse subbehaviors. They are called once when overlapping with the targeted object or actor to start invoking
	/// the associated behavior
	/// </summary>
	void HobbyTransition(); //Transitions into the Hobby subbehavior from the Follow behavior.
	void ConverseTransition(); //Transitions into the Converse subbehavior from the Follow behavior.


	/// <summary>
	/// These functions all define the basic behaviors that are called each minute that passes within specific physical villager states. This includes the WorkingBehaviors, WalkingBehaviors, TalkingBehaviors,
	/// IdleBehaviors, HobbyBehaviors, RelaxingBehaviors, UnconsciousBehaviors, and AsleepBehaviors functions.
	/// </summary>
	void WorkingBehaviors(); // Behaviors called each minute while the villager is in the Working state.
	void WalkingBehaviors(); // Behaviors called each minute while the villager is in the Walking state.
	void TalkingBehaviors(); // Behaviors called each minute while the villager is in the Talking state.
	void IdleBehaviors(); // Behaviors called each minute while the villager is in the Idle state.
	void HobbyBehaviors(); // Behaviors called each minute while the villager is in the Hobby state.
	void RelaxingBehaviors(); // Behaviors called each minute while the villager is in the Relaxing state.
	void UnconsciousBehaviors(); // Behaviors called each minute while the villager is in the Unconscious state.
	void AsleepBehaviors(); // Behaviors called each minute while the villager is in the Asleep state.
	typedef void (ABaseVillager::* StateBehaviors)(void); //This is an array of function pointers to allow easy calling of the relevant behavior with no unnecessary State checking
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



	/// <summary>
	/// These functions and variable work to carry out the Villager-initiated conversation subbehavior.
	/// </summary>	
	UFUNCTION(BlueprintCallable)
	void DecrementConversations(); //Decrements the number of responses that the NPC needs to generate while talking to the player.
	
	UFUNCTION(BlueprintCallable)
	bool CanTalk(); //Checks whether or not the villager is able to talk to the player.
	
	UFUNCTION(BlueprintCallable)
	bool CanStartConversation(); //Checks whether or not the villager is able to talk with another villager.
	
	UFUNCTION(BlueprintCallable)
	void InitiateConversation(); //Actually sets up relevant variables and states for a villager-initiated conversation.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlayerConversationInitiated; //Keeps track of whether or not the villager has initiated a conversation with the player.
	
	AActor* ChoosePartner(); //Chooses a random conversation partner for the villager to talk to.
	void EndInitiatedDialog(); //Ends an NPC-initiated conversation on the calling NPC's end.
	AActor* ConversationPartner; //A pointer to an actor, representing a conversation partner.



	/// <summary>
	/// These variables and functions define meander subbehaviors.
	/// </summary>
	float IdleSeconds = 4.0f; //The seconds of delay before the villager actually walks to the new point.
	float IdleRadius = 1000; //The radius around which the villager will check for a random point to meander to.
	FTimerHandle IdleTimer; //The timer handle that keeps track of the delay before inititating the meander behavior.
	void IdleLoafing(); //This is the actual implementation of the meander behavior, where the villager chooses a random point to travel to.


	/// <summary>
	/// These functions are called on a minute-to-minute basis to update potentially consistently updating variables, like energy and state.
	/// </summary>
	void UpdateEnergy(bool UpdateLevels); //Updates the energy of the NPC based on their state and relevant variables.
	void UpdateStatus(); //Updates the status of the NPC, called every ingame minute. It contains several other functions that adjust specific variables.
	bool RecoverEnergy(); //Determines whether or not energy should be recovered, switching to relevant states if necessary.
	bool CheckToSleep(); //Checks if the villager needs to go to sleep based on the current time.


	/// <summary>
	/// These variables and functions are used to carry out job-specific behavior.
	/// </summary>
	void CheckOnJob(const UWeekday& CurrentDay); // Checks on the job-relevant times, invoking their behaviors whenever activated.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OvertimeMinutes = 0; // This is the number of minutes that the villager needs to continue working past the normal endtime of the job.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimestamp JobOvertimeEnd; //This is the timestamp representing the actual time the villager can be off work for the day, the normal end time adjusted by the amount of overtime.


	/// <summary>
	/// These are hobby-relevant variables and functions.
	/// </summary>
	bool AttemptHobbySelection(); //Tries to select a random hobby for the villager to invoke
	int TargetHobby = Hobbies.Num(); //This integer represents the current hobby that's being invoked.
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // A string specifying the name of the hobby being invoked, checked when performing hobby animations.
	FString CurrentHobby = "";


	/// <summary>
	/// These functions are specific to normal, player-initiated conversation behaviors.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void Talk(); //This function starts talking behavior for the villager.
	UFUNCTION(BlueprintCallable)
	void EndDialog(); // This function wraps up conversation with the NPC by returning them to their previous state.


	/// <summary>
	/// Relevant functions to affinity adjustment and updating.
	/// </summary>
	void UpdateAffinity(); //Updates the affinity, clamping it if necessary, on the villager.

	TArray<TArray<FString>*> DialogBanks;
	/// <summary>
	/// These functions and variables all carry out and define the process of reading in text files to fill villager dialog banks.
	/// </summary>
	bool FindSubBank(FString& StringValue); // Goes through the dialogue file, checking for specific sub-banks and populating them as needed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueFile") // Specifies the name of the dialogue file which contains the villager's responses.
	bool UseDialogueFile = false; //Specifies whether or not an external dialogue file is to be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueFile")
	FString FilePath;

	UFUNCTION(BlueprintCallable)
	void LoadInDialog();

	UPROPERTY(BlueprintReadWrite)
	bool DialogLoaded = false;
};
