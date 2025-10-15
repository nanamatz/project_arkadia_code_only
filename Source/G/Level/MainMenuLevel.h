// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "G/UI/MainMenuWidget.h"
#include "G/Level/MainCameraDirector.h"
#include "MainMenuLevel.generated.h"

/**
 * 
 */

UCLASS()
class G_API AMainMenuLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> _MainMenuWidget;
	UPROPERTY(VisibleInstanceOnly, Category = "Widget")
	UMainMenuWidget* MainMenuWidget;
	UPROPERTY(EditAnyWhere)
	AActor* MainCam;
};
