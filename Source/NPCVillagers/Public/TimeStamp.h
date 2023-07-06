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
		int newHours = Hour - RHSTimestamp.Hour;
		int NewMinutes = Minute - RHSTimestamp.Minute;
	
		if (NewMinutes < 0)
		{
			newHours -= (1 + (NewMinutes / 60));
			NewMinutes = (NewMinutes % 60);
			if (NewMinutes < 0)
			{
				NewMinutes += ((-NewMinutes / 60)+1)*60;
			}
		}
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
			if (RHSTimestamp.Minute > Minute)
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

	bool operator>=(FTimestamp RHSTimestamp)
	{
		return (*this > RHSTimestamp || *this == RHSTimestamp);
	}

	bool operator<=(FTimestamp RHSTimestamp)
	{
		return (*this < RHSTimestamp || *this == RHSTimestamp);
	}

	bool BetweenTwoTimes(FTimestamp FirstValue, FTimestamp SecondValue)
	{
		FTimestamp Difference;
		bool IsBetween = false;

		if (FirstValue > SecondValue)
		{
			if (*this >= FirstValue || *this <= SecondValue)
			{
				IsBetween = true;
			}
			else
			{
				IsBetween = false;
			}
		}
		else
		{
			Difference = SecondValue - FirstValue;
			if (*this > FirstValue && *this < Difference + FirstValue)
			{
				IsBetween = true;
			}
			else
			{
				IsBetween = false;
			}
		}
		return IsBetween;
	}

	void GenerateRandomTime(FTimestamp MinTime, FTimestamp MaxTime, FTimestamp& StartTime, FTimestamp& LastTime)
	{
		auto TimeRange = MaxTime - MinTime;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Range time %d:%d"), TimeRange.Hour, TimeRange.Minute));
		int ProposedHours = 0;
		int ProposedMinutes = 0;

		if (TimeRange.Hour >= 1)
		{
			ProposedHours = FMath::RandRange(0, TimeRange.Hour);
			if (ProposedHours == TimeRange.Hour)
			{
				ProposedMinutes = FMath::RandRange(0, TimeRange.Minute);
			}
			else
			{
				ProposedMinutes = FMath::RandRange(0, 59);
			}
		}
		else
		{
			ProposedMinutes = FMath::RandRange(0, TimeRange.Minute);
		}
		FTimestamp FinalAmount = { ProposedHours, ProposedMinutes };
		FinalAmount = FinalAmount + MinTime;

		StartTime = *this;
		LastTime = FinalAmount + *this;
		if (LastTime.Hour >= 24)
		{
			LastTime.Hour -= 24;
		}
	}
};