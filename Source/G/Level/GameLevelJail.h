// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameLevelJail.generated.h"

/**
 * 
 */
UCLASS()
class G_API AGameLevelJail : public ALevelScriptActor
{
	GENERATED_BODY()


protected:
	AGameLevelJail();
	virtual void BeginPlay() override;
	


	UPROPERTY()
	class AGGameMode* GameMode;
};
