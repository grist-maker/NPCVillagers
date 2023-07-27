// Fill out your copyright notice in the Description page of Project Settings.
#include "Hobby.h"

void AHobby::BeginPlay()
{
	Super::BeginPlay();
	LoadInParameters();
}
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

void AHobby::LoadInParameters()
{
	if (UseParameterFile)
	{
		TArray<FString> TextLine;
		const TCHAR* CharFilePath = *ParameterFilePath;
		IPlatformFile* PlatformFile = FPlatformFileManager::Get().FindPlatformFile(CharFilePath);

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(CharFilePath))
		{
			FFileHelper::LoadFileToStringArray(TextLine, CharFilePath);
			
			int ParameterIndex = 20;

			for (int i = 0; i < TextLine.Num(); i++)
			{
				if (!FindParameter(TextLine[i], ParameterIndex) && ParameterIndex < 4)
				{
					if (ParameterIndex < 3) //This is parsing the Days enum value by converting its value to a string representation
					{
						auto Index = TextLine[i].Find(" ");
						if (Index < TextLine[i].Len())
						{
							static_cast<FTimestamp*>(Parameters[ParameterIndex])->Hour = FCString::Atoi(*TextLine[i].Left(Index));
							static_cast<FTimestamp*>(Parameters[ParameterIndex])->Minute = FCString::Atoi(*TextLine[i].Right(TextLine[i].Len() - Index));
						}
					}
					else //This is simply setting the string title
					{
						*static_cast<FString*>(Parameters[ParameterIndex]) = TextLine[i];
					}
				}
			}
		}
	}
}

bool AHobby::FindParameter(FString& StringValue, int& ParameterIndex)
{
	if (StringValue.Equals("MinHobbyTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 0;
		return true;
	}
	if (StringValue.Equals("MaxHobbyTime", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 1;
		return true;
	}
	if (StringValue.Equals("HobbyDelay", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 2;
		return true;
	}
	if (StringValue.Equals("Name", ESearchCase::IgnoreCase))
	{
		ParameterIndex = 3;
		return true;
	}
	return false;
}