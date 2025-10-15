// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Map/WorldmapWidget.h"

#include "PointOfInterestComponent.h"
#include "PointOfInterestWidget.h"
#include "WorldmapPointOfInterestWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Image.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/InGameDataManager.h"
#include "Kismet/GameplayStatics.h"

class UPointOfInterestComponent;
class UBorderSlot;
class UPlayerIconWidget;

void UWorldmapWidget::UpdatePlayerIcon(FVector2D& map_loc, float& zoom)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerCharacter* control_char = GameInstance->GetInGameDataManager()->GetControlCharacter();

	FVector player_loc = control_char->GetActorLocation();
	FRotator player_rot = control_char->GetActorRotation();

	float worldmap_x = ((player_loc.Y - map_loc.Y * 30) / worldmap_dimension * 1600.f) * (1 / zoom);
	float worldmap_y = -((player_loc.X - map_loc.X * 30) / worldmap_dimension * 900.f) * (1 / zoom);

	if (IsValid(player_icon))
	{
		player_icon->SetRenderTranslation({worldmap_x, worldmap_y});
		player_icon->SetRenderTransformAngle(player_rot.Yaw);
	}
}

void UWorldmapWidget::UpdateAllPoiIcon(FVector2D& map_loc, float& zoom)
{
	for (UWidget* _poi_widget : map_overlay->GetAllChildren())
	{
		UWorldmapPointOfInterestWidget* poi_widget = Cast<UWorldmapPointOfInterestWidget>(_poi_widget);
		if (IsValid(poi_widget))
		{
			LOG(Log, "");
			poi_widget->Update(map_loc, worldmap_dimension, zoom);
		}
	}
}

void UWorldmapWidget::UpdatePoiIcon(UUserWidget* poi_icon)
{
}

void UWorldmapWidget::Set()
{
}

void UWorldmapWidget::ChangeToStage1()
{
	if(current_map == "stage1")
	{
		return;	
	}

	current_map = "stage1";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), stage1_map);
}

void UWorldmapWidget::ChangeToCaveB1()
{
	if(current_map == "caveb1")
	{
		return;	
	}

	current_map = "caveb1";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), cave_b1_map);
}

void UWorldmapWidget::ChangeToCaveB2()
{	if(current_map == "caveb2")
{
	return;	
}

	current_map = "caveb2";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), cave_b2_map);
}

void UWorldmapWidget::ChangeToPalace()
{
	if(current_map == "palace")
	{
		return;	
	}

	current_map = "palace";
	map->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), palace_map);
}

void UWorldmapWidget::Init()
{
}

void UWorldmapWidget::On(FVector2D& map_loc, float& zoom)
{
	Update(map_loc, zoom);
}

void UWorldmapWidget::Update(FVector2D& map_loc, float& zoom)
{
	UpdateAllPoiIcon(map_loc, zoom);
}

void UWorldmapWidget::AddPoi(AActor* actor)
{
	UPointOfInterestComponent* poi_comp = Cast<UPointOfInterestComponent>(
		actor->GetComponentByClass(UPointOfInterestComponent::StaticClass()));

	if (IsValid(poi_comp))
	{
		UWorldmapPointOfInterestWidget* new_poi = CreateWidget<UWorldmapPointOfInterestWidget>(this, bp_poi_widget);

		new_poi->IsStatic = poi_comp->is_static;
		new_poi->SetOwner(actor);

		UOverlaySlot* overlay_slot = map_overlay->AddChildToOverlay(new_poi);
		overlay_slot->SetHorizontalAlignment(HAlign_Center);
		overlay_slot->SetVerticalAlignment(VAlign_Center);
	}
}

void UWorldmapWidget::AddPlayerIcon(ECharacterName character)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerCharacter* control_char = GameInstance->GetInGameDataManager()->GetPlayerCharacter(character);
	LOG(Log, "");

	if (control_char)
	{
		LOG(Log, "");
		player_icon = CreateWidget<UWorldmapPointOfInterestWidget>(this, bp_poi_widget);
		player_icon->SetOwner(control_char);

		UOverlaySlot* overlay_slot = map_overlay->AddChildToOverlay(player_icon);
		overlay_slot->SetHorizontalAlignment(HAlign_Center);
		overlay_slot->SetVerticalAlignment(VAlign_Center);
	}
}

void UWorldmapWidget::Off()
{
}