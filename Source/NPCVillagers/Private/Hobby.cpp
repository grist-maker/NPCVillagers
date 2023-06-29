// Fill out your copyright notice in the Description page of Project Settings.
#include "Hobby.h"

void AHobby::DailyReset()
{
	ActiveHobbyEndTime = { 0, 0 };
	LastHobbyStartTime = { 0, 0 };
}
bool AHobby::CanInvoke(FTimestamp CurrentTime)
{
	FTimestamp LowestTime{ 0, 0 };
	if(CurrentTime > ActiveHobbyEndTime + HobbyDelay || ActiveHobbyEndTime == LowestTime)
	{
		return true;
	}
	return false;
}