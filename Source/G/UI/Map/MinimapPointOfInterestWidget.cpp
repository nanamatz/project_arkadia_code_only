// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Map/MinimapPointOfInterestWidget.h"

#include "Components/Image.h"
#include "G/GGameInstance.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/InGameDataManager.h"
#include "G/UI/InGameUI/InGameUI.h"
#include "MapWidget.h"
#include "Kismet/GameplayStatics.h"

class UGGameInstance;

void UMinimapPointOfInterestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(owner))
	{
		RemovePOI();

		return;
	}

	if(owner->IsHidden())
	{
		RemovePOI();

		return;
	}
	
	UMapWidget* minimap = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetInGameUI()->
		GetMinimap();

	if (minimap->GetControlCharacter())
	{
		float minimap_dimension = minimap->GetDimension() / minimap->GetMapSize() * minimap->GetZoom();

		float minimap_y = (minimap->GetControlCharacter()->GetActorLocation().X - owner->GetActorLocation().X) /
			minimap_dimension;
		float minimap_x = -(minimap->GetControlCharacter()->GetActorLocation().Y - owner->GetActorLocation().Y) /
			minimap_dimension;
		FVector2D MinimapLoc(minimap_x, minimap_y);

		if (MinimapLoc.Size() <= 130.f)
		{
			custom_image->SetVisibility(ESlateVisibility::Visible);
			SetRenderTranslation(FindCoord(MinimapLoc.Size(), FindAngle(MinimapLoc)));
		}
		else
		{
			custom_image->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

float UMinimapPointOfInterestWidget::FindAngle(FVector2D& b)
{
	// Player is always in center on the minimap.
	float X = 0 - b.X;
	float Y = 0 - b.Y;

	return atan2(Y, X);
}

FVector2D UMinimapPointOfInterestWidget::FindCoord(float radius, float degrees)
{
	float ClampRadius = FMath::Clamp(radius, 0.f, 130.f);
	float X = -ClampRadius * FMath::Cos(degrees);
	float Y = -ClampRadius * FMath::Sin(degrees);

	return FVector2D(X, Y);
}

void UMinimapPointOfInterestWidget::RemovePOI()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetInGameDataManager()->RemovePOIMap(owner_name);
	RemoveFromParent();
}
