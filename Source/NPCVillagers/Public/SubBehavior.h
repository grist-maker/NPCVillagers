#pragma once
#include "Timestamp.h"
#include "CoreMinimal.h"
#include "SubBehavior.generated.h"

class ABaseVillager;

UENUM(BlueprintType)
enum class USubbehaviors : uint8
{
	Meander,
	Hobby,
	Converse
};

USTRUCT(BlueprintType)
struct FSubbehavior
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USubbehaviors Behavior;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Percent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp MinTime{ 0, 0 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimestamp MaxTime{ 0, 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinExecutions = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxExecutions = 0;

	FTimestamp StartTime{ 0, 0 };
	FTimestamp LastTime{ 0, 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentExecutions = 0;
};