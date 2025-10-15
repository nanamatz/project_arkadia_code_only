// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointOfInterestWidget.h"
#include "Blueprint/UserWidget.h"
#include "WorldmapPointOfInterestWidget.generated.h"

/**
 * 
 */
UCLASS()
class G_API UWorldmapPointOfInterestWidget : public UPointOfInterestWidget
{
	GENERATED_BODY()

public:
	void Update(FVector2D& map_loc, float& worldmap_dimension, float& zoom);
};
