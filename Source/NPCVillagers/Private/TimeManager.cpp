// Fill out your copyright notice in the Description page of Project Settings.
#include "TimeManager.h"

void ATimeManager::BeginPlay()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseVillager::StaticClass(), Villagers);
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(SecondCounter, this, &ATimeManager::NewMinute, 0.2f, true, 0.2f);
}

void ATimeManager::NewMinute()
{
	FTimestamp NewTime;
	NewTime.Hour = 0;
	NewTime.Minute = 1;

	ProgressTime(NewTime);
	UpdateVillagers();
}

void ATimeManager::ProgressTime(FTimestamp& Progress)
{
	int AdditionalHours = 0;
	int AdditionalDays = 0;

	if (CurrentTime.Minute + Progress.Minute < 60)
	{
		CurrentTime.Minute += Progress.Minute;
	}
	else
	{
		int minuteSum = CurrentTime.Minute + Progress.Minute;
		AdditionalHours = minuteSum / 60;
		CurrentTime.Minute = minuteSum % 60;
	}

	if (CurrentTime.Hour + Progress.Hour + AdditionalHours < 24)
	{
		CurrentTime.Hour += (AdditionalHours + Progress.Hour);
	}
	else
	{
		int hourSum = CurrentTime.Hour + Progress.Hour + AdditionalHours;
		AdditionalDays = hourSum / 24;
		CurrentTime.Hour = hourSum % 24;

		for (int i = 0; i < Villagers.Num(); i++)
		{
			if (Villagers[i] != nullptr)
			{
				ABaseVillager* NPC = Cast<ABaseVillager>(Villagers[i]);
				
				NPC->DoneWork = false;
			}
		}
	}
	int ExtraDays = (AdditionalDays) % 7;
	
	if (static_cast<int>(CurrentDay) + AdditionalDays > 7)
	{
		CurrentDay = static_cast<UWeekday>((AdditionalDays  + static_cast<int>(CurrentDay))% 7);
	}
	else
	{
		CurrentDay = static_cast<UWeekday>(static_cast<int>(CurrentDay) + AdditionalDays);
	}

	Cast<APlayerVillager>(Player)->GetDefaultSubobjectByName("DateTime");

	if (AdditionalHours > 0)
	{
		if (CurrentTime.Hour < 12)
		{
			if (CurrentTime.Hour == 0)
			{
				UpdateLighting(.75f / 4.0f);
			}
			else
			{
				UpdateLighting(CurrentTime.Hour / 4.0f);
			}
		}
		else
		{
			UpdateLighting(3 - (CurrentTime.Hour-12)/4.0f);
		}
	}
}

void ATimeManager::UpdateVillagers()
{
	JobUpdate();
	EnergyUpdate();
}

void ATimeManager::JobUpdate()
{
	for (int i = 0; i < Villagers.Num(); i++)
	{
		if (Villagers[i] != nullptr)
		{
			ABaseVillager* NPC = Cast<ABaseVillager>(Villagers[i]);
			if (NPC->State != UState::Talking && !NPC->DoneWork)
			{
				if (NPC->Career->CommuteTime.Hour <= CurrentTime.Hour && NPC->Career->CommuteTime.Minute <= CurrentTime.Minute && !NPC->Commuting && !NPC->AtWork && NPC->Career->Days.Contains(CurrentDay) && (NPC->Career->EndTime.Hour >= CurrentTime.Hour ))
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Commuting"));
					NPC->Commuting = true;
					NPC->State = UState::Walking;
					NPC->NPCAIController->MoveToActor(NPC->Career->Workstation, 0, true);
				}
				else if (NPC->Career->EndTime.Hour == CurrentTime.Hour && NPC->Career->EndTime.Minute <= CurrentTime.Minute && NPC->AtWork)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Leaving Job"));
					NPC->AtWork = false;
					NPC->DoneWork = true;
					NPC->State = UState::Idle;
				}
				else if (NPC->Commuting && NPC->IsOverlappingActor(NPC->Career->Workstation))
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Beginning work"));
					NPC->AtWork = true;
					NPC->Commuting = false;
					NPC->State = UState::Working;
					NPC->NPCAIController->StopMovement();
				}
			}
		}
	}
}

void ATimeManager::EnergyUpdate()
{
	for (int i = 0; i < Villagers.Num(); i++)
	{
		if (Villagers[i] != nullptr)
		{
			ABaseVillager* NPC = Cast<ABaseVillager>(Villagers[i]);
			if (NPC->State == UState::Working)
			{
				NPC->Energy -= NPC->JobEnergyDrain;
			}
			else if (NPC->State == UState::Hobby)
			{
				NPC->Energy -= NPC->HobbyEnergyDrain;
			}
			else if (NPC->State == UState::Relaxing)
			{
				NPC->Energy += NPC->RelaxEnergyGain;
			}
			else if (NPC->State == UState::Asleep)
			{
				NPC->Energy += NPC->SleepEnergyGain;
			}
			else if (NPC->State == UState::Unconscious)
			{
				NPC->Energy += NPC->UnconsciousEnergyGain;
			}
			else
			{
				NPC->Energy -= NPC->BaseEnergyDrain;
			}
			NPC->UpdateStatus();
		}
	}
}


void ATimeManager::UpdateLighting(float Lux)
{
	if (Sun != nullptr)
	{
		Sun->GetLightComponent()->SetIntensity(Lux);
	}
}