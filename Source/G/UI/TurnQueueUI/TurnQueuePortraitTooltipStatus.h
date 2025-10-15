// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnQueuePortraitTooltipStatus.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;

/**
 * 
 */
UCLASS()
class G_API UTurnQueuePortraitTooltipStatus : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* status_img;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* status_text;

public:
	void SetStatus(UTexture2D& img, FText text);
};
