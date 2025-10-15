// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/InGameUI/PlayerPortrait.h"
#include "CircularPlayerPortrait.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class G_API UCircularPlayerPortrait : public UPlayerPortrait
{
	GENERATED_BODY()

public:
	void Init();
	void SetPortrait(APlayerCharacter* pc);
	void On();
	void Off();
	void UpdatePortrait();
};
