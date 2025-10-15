// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Map/WorldmapPointOfInterestWidget.h"

#include "Components/Image.h"
#include "G/G.h"

void UWorldmapPointOfInterestWidget::Update(FVector2D& map_loc, float& worldmap_dimension, float& zoom)
{
	if (IsValid(owner))
	{
		if(owner->IsHidden())
		{
			RemoveFromViewport();
		}
		FVector owner_loc = owner->GetActorLocation();

		float worldmap_x = ((owner_loc.Y - map_loc.Y * 30) / worldmap_dimension * 1600.f) * (1 / zoom);
		float worldmap_y = -((owner_loc.X - map_loc.X * 30) / worldmap_dimension * 900.f) * (1 / zoom);

		if (abs(worldmap_x) < 800.f && abs(worldmap_y) < 450.f)
		{
			custom_image->SetVisibility(ESlateVisibility::Visible);
			SetRenderTranslation({worldmap_x, worldmap_y});
		}
		else
		{
			custom_image->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		RemoveFromViewport();
	}
}
