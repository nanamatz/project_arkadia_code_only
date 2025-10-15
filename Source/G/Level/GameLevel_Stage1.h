// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameLevel_Stage1.generated.h"

/**
 * 
 */
UCLASS()
class G_API AGameLevel_Stage1 : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	AGameLevel_Stage1();
	virtual void BeginPlay() override;
};
