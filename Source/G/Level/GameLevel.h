// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameLevel.generated.h"

/**
 * ���� ��忡�� ������ ĳ���� ��ġ
 */
UCLASS()
class G_API AGameLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	AGameLevel();
	virtual void BeginPlay() override;

	UPROPERTY()
	class AGGameMode* GameMode;
};
