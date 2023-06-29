// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeStamp.generated.h"

/// <summary>
/// The Timestamp struct is used to represent a point of time via an hour and minute portion, or an amount of time passed depending on use.
/// </summary>
USTRUCT(BlueprintType)
struct FTimestamp
{
public:
	GENERATED_BODY()
	/// <summary>
	/// The hour preserved by the timestamp.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Hour = 0;

	/// <summary>
	/// The minute preserved by the timestamp.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Minute = 0;

	FTimestamp operator=(FTimestamp NewTimestamp)
	{
		Hour = NewTimestamp.Hour;
		Minute = NewTimestamp.Minute;
		return *this;
	}

	bool operator==(FTimestamp RHSTimestamp)
	{
		return (Hour == RHSTimestamp.Hour && Minute == RHSTimestamp.Minute);
	}

	FTimestamp operator+(FTimestamp RHSTimestamp)
	{
		int newHours = 0;
		int NewMinutes = Minute + RHSTimestamp.Minute;
		if (NewMinutes >= 60)
		{
			newHours = NewMinutes / 60;
			NewMinutes = NewMinutes % 60;
		}
		newHours += (Hour + RHSTimestamp.Hour);
		FTimestamp newTimestamp{ newHours, NewMinutes };
	
		return newTimestamp;
	}

	FTimestamp operator-(FTimestamp RHSTimestamp)
	{
		int newHours = 0;
		int NewMinutes = Minute - RHSTimestamp.Minute;
		if (NewMinutes <= 0)
		{
			newHours = Hour - (NewMinutes / 60);
			NewMinutes = (NewMinutes % 60);
			if (NewMinutes < 0)
			{
				NewMinutes += ((-NewMinutes / 60)+1)*60;
			}
		}
		newHours -= RHSTimestamp.Hour;
		FTimestamp newTimestamp{ newHours, NewMinutes };

		return newTimestamp;
	}

	bool operator<(FTimestamp RHSTimestamp)
	{
		if (RHSTimestamp.Hour > Hour)
		{
			return true;
		}
		if (RHSTimestamp.Hour == Hour)
		{
			if (RHSTimestamp.Minute < Minute)
			{
				return true;
			}
		}
		return false;
	}

	bool operator>(FTimestamp RHSTimestamp)
	{
		 if (Hour > RHSTimestamp.Hour)
		 {
			return true;
		 }
		if (RHSTimestamp.Hour == Hour)
		{
			if (Minute > RHSTimestamp.Minute)
			{
				return true;
			}
		}
		return false;
	}
};