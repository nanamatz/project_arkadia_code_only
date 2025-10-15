// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/PlayerCharacter.h"
#include "RuthCindaClass.generated.h"

/**
 * 
 */
UCLASS()
class G_API ARuthCindaClass : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ARuthCindaClass();
};
