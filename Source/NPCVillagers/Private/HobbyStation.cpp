// Fill out your copyright notice in the Description page of Project Settings.

#include "HobbyStation.h"
#include "BaseVillager.h"

// Sets default values
AHobbyStation::AHobbyStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AHobbyStation::AddHobbyPerformer(ABaseVillager* NewPerformer)
{
	if (!HobbyPerformers.Contains(NewPerformer))
	{
		HobbyPerformers.Add(NewPerformer);
	}
}

void AHobbyStation::RemoveHobbyPerformer(ABaseVillager* Performer)
{
	if (!HobbyPerformers.Contains(Performer))
	{
		HobbyPerformers.Remove(Performer);
	}
}

// Called when the game starts or when spawned
void AHobbyStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHobbyStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}