// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/PlayerCharacter.h"
#include "CardenioClass.generated.h"

/**
 * 
 */
UCLASS()
class G_API ACardenioClass : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	FTimerHandle SkillTimerHandle;

public:
	ACardenioClass();

	virtual void BeginPlay() override;
};
