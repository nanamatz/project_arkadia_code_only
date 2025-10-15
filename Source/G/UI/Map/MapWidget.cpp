// Fill out your copyright notice in the Description page of Project Settings.


#include "MapWidget.h"
#include "PlayerIconWidget.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "G/GGameMode.h"
#include <Kismet/GameplayStatics.h>

#include "MinimapPointOfInterestWidget.h"
#include "PointOfInterestComponent.h"
#include "PointOfInterestWidget.h"
#include "G/G.h"


class UPointOfInterestWidget;

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Dimensions"), dimensions);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Zoom"), zoom);
	/*render_target_texture = render_target->ConstructTexture2D(render_target, "minimap_texture", RF_NoFlags,
	                                                          CTF_DeferCompression);
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), render_target_texture);*/
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateControlCharacter();

	if (ControlCharacter)
	{
		player_location = ControlCharacter->GetActorLocation();
		player_rotation = ControlCharacter->GetActorRotation();

		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Y"), player_location.X);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("X"), player_location.Y);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Zoom"), zoom);

		player_icon->SetRenderTransformAngle(player_rotation.Yaw);
	}
}

void UMapWidget::Init()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();

	player_icon = CreateWidget<UPlayerIconWidget>(this, _player_icon);
	UOverlaySlot* overlay_slot = map_overlay->AddChildToOverlay(player_icon);

	overlay_slot->SetHorizontalAlignment(HAlign_Center);
	overlay_slot->SetVerticalAlignment(VAlign_Center);
}

float UMapWidget::GetDimension()
{
	return dimensions;
}

float UMapWidget::GetZoom()
{
	return zoom;
}

float UMapWidget::GetMapSize()
{
	return MapSize;
}

APlayerCharacter* UMapWidget::GetControlCharacter()
{
	return ControlCharacter;
}

void UMapWidget::ChangeToStage1()
{
	if(current_map == "stage1")
	{
		return;	
	}

	current_map = "stage1";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), stage1_map);
}

void UMapWidget::ChangeToCaveB1()
{
	if(current_map == "caveb1")
	{
		return;	
	}

	current_map = "caveb1";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), cave_b1_map);
}

void UMapWidget::ChangeToCaveB2()
{
	if(current_map == "caveb2")
	{
		return;	
	}

	current_map = "caveb2";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), cave_b2_map);
}

void UMapWidget::ChangeToPalace()
{
	if(current_map == "palace")
	{
		return;	
	}

	current_map = "palace";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), palace_map);
}


void UMapWidget::UpdateControlCharacter()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
}

void UMapWidget::AddPOI(AActor* Actor)
{	
	UPointOfInterestComponent* poi_comp = Cast<UPointOfInterestComponent>(
		Actor->GetComponentByClass(UPointOfInterestComponent::StaticClass()));

	if (IsValid(poi_comp))
	{
		UMinimapPointOfInterestWidget* new_poi = CreateWidget<UMinimapPointOfInterestWidget>(this, BP_POIWidget);

		if (IsValid(new_poi))
		{
			new_poi->IsStatic = poi_comp->is_static;
			new_poi->SetOwner(Actor);

			UOverlaySlot* overlay_slot = map_overlay->AddChildToOverlay(new_poi);
			overlay_slot->SetHorizontalAlignment(HAlign_Center);
			overlay_slot->SetVerticalAlignment(VAlign_Center);
		}
	}
}

UPointOfInterestWidget* UMapWidget::GetPOI(AActor* owner)
{
	LOG(Log, "%p", owner)
	
	for(auto child : map_overlay->GetAllChildren())
	{
		UPointOfInterestWidget* poi = Cast<UPointOfInterestWidget>(child);

		if(IsValid(poi))
		{
			if(poi->GetOwner() == owner)
			{
				return poi;
			}
		}
	}
	
	return nullptr;
}

