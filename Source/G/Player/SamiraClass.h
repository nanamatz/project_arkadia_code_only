// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/PlayerCharacter.h"
#include "SamiraClass.generated.h"

/**
 * 
 */
UCLASS()
class G_API ASamiraClass : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASamiraClass();
};
