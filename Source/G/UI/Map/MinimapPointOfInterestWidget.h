// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointOfInterestWidget.h"
#include "Blueprint/UserWidget.h"
#include "MinimapPointOfInterestWidget.generated.h"

/**
 * 
 */
UCLASS()
class G_API UMinimapPointOfInterestWidget : public UPointOfInterestWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float FindAngle(FVector2D& b);
	FVector2D FindCoord(float radius, float degrees);
	void RemovePOI();
};
