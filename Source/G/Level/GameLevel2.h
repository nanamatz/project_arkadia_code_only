// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameLevel2.generated.h"

/**
 * 
 */
UCLASS()
class G_API AGameLevel2 : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	AGameLevel2();

	virtual void BeginPlay() override;
};
