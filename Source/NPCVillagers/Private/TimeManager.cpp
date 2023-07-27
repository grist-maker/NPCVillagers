// Fill out your copyright notice in the Description page of Project Settings.
#include "TimeManager.h"

void ATimeManager::BeginPlay()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseVillager::StaticClass(), Villagers);

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(CoworkerDelay, this, &ATimeManager::CoworkerSetup, 1, false);
	GetWorldTimerManager().SetTimer(SecondCounter, this, &ATimeManager::NewMinute, 0.2f, true, 0.2f);
}

void ATimeManager::CoworkerSetup()
{
	for (int i = 0; i < Villagers.Num(); i++)
	{
		auto NewVillager = Cast<ABaseVillager>(Villagers[i]);
		if (NewVillager != nullptr)
		{
			auto Career = NewVillager->Career;
			if (Career != nullptr)
			{
				if (Career->JobTitle == "Software Engineer")
				{
					SoftwareEngineers.Add(NewVillager);
				}
				else if (Career->JobTitle == "Gardener")
				{
					Gardeners.Add(NewVillager);
				}
				else if (Career->JobTitle == "Merchant")
				{
					Merchants.Add(NewVillager);
				}
				else if (Career->JobTitle == "Fisher")
				{
					Fishers.Add(NewVillager);
				}
			}
		}
	}
}

void ATimeManager::NewMinute()
{
	FTimestamp NewTime;
	NewTime.Hour = 0;
	NewTime.Minute = 1;

	ProgressTime(NewTime);
	UpdateVillagers();

	if (!MorningMoodUpdate || !NoonMoodUpdate || !EveningMoodUpdate)
	{
		CheckForMoodUpdate();
	}
}

void ATimeManager::CheckForMoodUpdate()
{
	if (NextMoodTime.Hour <= CurrentTime.Hour && NextMoodTime.Minute < CurrentTime.Minute)
	{
		if (!MorningMoodUpdate)
		{
			MorningMoodUpdate = true;
			NextMoodTime = NoonMoodTime;
		}
		else if (!NoonMoodUpdate)
		{
			NoonMoodUpdate = true;
			NextMoodTime = EveningMoodTime;
		}
		else if (!EveningMoodUpdate)
		{
			EveningMoodUpdate = true;
			NextMoodTime = MorningMoodTime;
		}

		for (int i = 0; i < Villagers.Num(); i++)
		{
			if (Villagers[i] != nullptr)
			{
				for (int j = 0; j < 4; j++)
				{
					int RandomMood = FMath::RandRange(0, 1);
					if (RandomMood == 0)
					{
						Cast<ABaseVillager>(Villagers[i])->PositiveMoodHit(((j+1)%2 +1 *(-1 * j%2)) * 15);
					}
					else
					{
						Cast<ABaseVillager>(Villagers[i])->NegativeMoodHit(((j + 1) % 2 + 1 * (- 1 * j % 2)) * 15);
					}
				}
			}
		}
	}
}

void ATimeManager::ProgressTime(FTimestamp& Progress)
{
	int AdditionalHours = 0;
	int AdditionalDays = 0;

	auto NewTime = Progress + CurrentTime;
	CurrentTime.Minute = NewTime.Minute;
	if(NewTime.Hour < 24)
	{
		CurrentTime = NewTime;
	}
	else
	{
		AdditionalDays = NewTime.Hour / 24;
		CurrentTime.Hour = NewTime.Hour % 24;

		MorningMoodUpdate = false;
		NoonMoodUpdate = false;
		EveningMoodUpdate = false;
		for (int i = 0; i < Villagers.Num(); i++)
		{
			if (Villagers[i] != nullptr)
			{
				ABaseVillager* NPC = Cast<ABaseVillager>(Villagers[i]);
				
				if (NPC != nullptr)
				{
					NPC->DoneWork = false;
				}
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
			NPC->CheckOnJob(CurrentDay);
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
			if (NPC != nullptr)
			{
				NPC->UpdateStatus();
				NPC->CurrentTime = CurrentTime;
			}
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