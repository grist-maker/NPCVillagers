// Fill out your copyright notice in the Description page of Project Settings.


#include "Job.h"
#include "DialogBank.h"

// Sets default values for this component's properties
AJob::AJob()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}

void AJob::BeginPlay()
{
	Super::BeginPlay();
	Responses = Cast<UDialogBank>(GetDefaultSubobjectByName("Dialog"));
	LoadInDialog();
	LoadInParameters();
}

void AJob::LoadInParameters()
{
	if (UseParameterFile)
	{
		TArray<FString> TextLine;

		FString FinalPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectContentDir().Append(ParameterFilePath));
		const TCHAR* CharFilePath = *FinalPath;

		IPlatformFile* PlatformFile = FPlatformFileManager::Get().FindPlatformFile(CharFilePath);

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(CharFilePath))
		{
			FFileHelper::LoadFileToStringArray(TextLine, CharFilePath);
			for (int i = 0; i < TextLine.Num(); i++)
			{
				if (!FindParameter(TextLine[i]) && ParameterIndex < Parameters.Num())
				{
					if (ParameterIndex == Parameters.Num() - 1) //This is parsing the Days enum value by converting its value to a string representation
					{
						for (int j = 0; j < static_cast<int>(UWeekday::Sunday); j++)
						{
							FString CurrentWeekday = UEnum::GetDisplayValueAsText(static_cast<UWeekday>(j)).ToString();
							if (TextLine[i].Equals(CurrentWeekday))
							{
								Days.Add(static_cast<UWeekday>(j));
							}
						}
					}
					else if (ParameterIndex == 0) //This is simply setting the string title
					{
						JobTitle = TextLine[i];
					}
					else //This is a mechanism to parse the timestamps
					{
						auto Index = TextLine[i].Find(" ");
						if (Index < TextLine[i].Len())
						{
							static_cast<FTimestamp*>(Parameters[ParameterIndex])->Hour = FCString::Atoi(*TextLine[i].Left(Index));
							static_cast<FTimestamp*>(Parameters[ParameterIndex])->Minute = FCString::Atoi(*TextLine[i].Right(TextLine[i].Len()-Index));
						}
					}
				}
			}
		}
	}
}

bool AJob::FindParameter(FString& StringValue)
{
	if (StringValue.Equals("JobTitle", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 0;
		return true;
	}
	if (StringValue.Equals("Days", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 6;
		return true;
	}
	if (StringValue.Equals("StartTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 1;
		return true;
	}
	if (StringValue.Equals("EndTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 2;
		return true;
	}
	if (StringValue.Equals("CommuteTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 3;
		return true;
	}
	if (StringValue.Equals("WakeTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 4;
		return true;
	}
	if (StringValue.Equals("SleepTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 5;
		return true;
	}
	return false;
}

void AJob::LoadInDialog()
{
	if (UseDialogueFile && Responses != nullptr)
	{
		TArray<FString> Dialogue;

		FString FinalPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectContentDir().Append(DialogueFilePath));
		const TCHAR* CharFilePath = *FinalPath;
		IPlatformFile* PlatformFile = FPlatformFileManager::Get().FindPlatformFile(CharFilePath);

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(CharFilePath))
		{
			FFileHelper::LoadFileToStringArray(Dialogue, CharFilePath);
			for (int i = 0; i < Dialogue.Num(); i++)
			{
				if (!FindSubBank(Dialogue[i]) && DialogBanks.Num() >= 1)
				{
					DialogBanks[DialogBanks.Num() - 1]->Add(Dialogue[i]);
				}
			}
		}
	}
}

bool AJob::FindSubBank(FString& StringValue)
{
	if (StringValue.Equals("HighAffinity", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HighAffinity);
		return true;
	}
	if (StringValue.Equals("MidAffinity", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->MidAffinity);
		return true;
	}
	if (StringValue.Equals("LowAffinity", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->LowAffinity);
		return true;
	}
	if (StringValue.Equals("HighEnergy", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HighEnergy);
		return true;
	}
	if (StringValue.Equals("MidEnergy", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->MidEnergy);
		return true;
	}
	if (StringValue.Equals("LowEnergy", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->LowEnergy);
		return true;
	}
	if (StringValue.Equals("HappyMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HappyMood);
		return true;
	}
	if (StringValue.Equals("SadMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->SadMood);
		return true;
	}
	if (StringValue.Equals("AngryMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->AngryMood);
		return true;
	}
	if (StringValue.Equals("ExcitedMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->ExcitedMood);
		return true;
	}
	if (StringValue.Equals("ScaredMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->ScaredMood);
		return true;
	}
	if (StringValue.Equals("BusyState", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->BusyState);
		return true;
	}
	if (StringValue.Equals("HurtReactions", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HurtReactions);
		return true;
	}
	if (StringValue.Equals("LikedGifts", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->LikedGifts);
		return true;
	}
	if (StringValue.Equals("MiddleGifts", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->MiddleGifts);
		return true;
	}
	if (StringValue.Equals("DislikedGifts", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->DislikedGifts);
		return true;
	}
	if (StringValue.Equals("Coworker", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->Coworker);
		return true;
	}
	if (StringValue.Equals("HurtCoworker", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HurtCoworker);
		return true;
	}
	return false;
}