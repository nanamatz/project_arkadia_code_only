// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionIcon.generated.h"

/**
 * 
 */
UCLASS()
class G_API UInteractionIcon : public UUserWidget
{
	GENERATED_BODY()
	
	bool isOn = false;
	FVector2D TargetPos = FVector2D(0.0f, 0.0f);
	FVector2D Offset = FVector2D(0.0f, 0.0f);
public:

	void OnInteractIcon(FVector2D pos, FVector2D offset);
	void OffInteractIcon();
	void SetVisible(bool vis);
	void CoordinateCalculation();
};
