#pragma once

#include "CoreMinimal.h"
#include "LevelUpInfo.generated.h"

USTRUCT()
struct FLevelUpInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	int32 LevelHP = 5;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	int32 LevelDamage = 0;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	int32 LevelCost = 1;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	float LevelMovePerCost = 1;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	float LevelAvoidRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	float LevelHitRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	float LevelCriticalRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	float LevelCriticalDamage = 1.0f;
};
