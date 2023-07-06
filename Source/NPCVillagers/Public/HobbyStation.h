// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HobbyStation.generated.h"

class ABaseVillager;

UCLASS()
class NPCVILLAGERS_API AHobbyStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHobbyStation();
	TArray<ABaseVillager*> HobbyPerformers;
	void AddHobbyPerformer(ABaseVillager* Performer);
	void RemoveHobbyPerformer(ABaseVillager* Performer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool PlayerStanding = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool PlayerPerforming = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
