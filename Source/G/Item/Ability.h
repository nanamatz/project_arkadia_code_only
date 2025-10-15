// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Util/Define.h"
#include "Ability.generated.h"

/**
 * 
 */

USTRUCT()
struct FAbility
{
	GENERATED_BODY()
	FAbility();
	~FAbility();

	UPROPERTY(EditAnywhere)
	TMap<EAbility, int32> Value =
	{
		{EAbility::Power, 0},
		{EAbility::Armor, 0},
		{EAbility::Strength, 0},
		{EAbility::Insight, 0},
		{EAbility::Swift, 0},
		{EAbility::Relaxed, 0},
		{EAbility::Careful, 0},
		{EAbility::Luck, 0}
	};

	FAbility& operator+(FAbility&);
	FAbility& operator+=(FAbility&);
	FAbility& operator-(FAbility&);
	FAbility& operator-=(FAbility&);
	bool operator<(FAbility&);
	bool operator>(FAbility&);
};
