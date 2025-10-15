// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "TestLevel.generated.h"

/**
 * 
 */
UCLASS()
class G_API ATestLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class AGGameMode* GameMode;

	UPROPERTY()
	class UGGameInstance* GameInstance;
};
