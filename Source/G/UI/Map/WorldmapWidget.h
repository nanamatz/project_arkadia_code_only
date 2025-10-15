// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Util/Define.h"
#include "WorldmapWidget.generated.h"

class UPlayerIconWidget;
class UWorldmapPointOfInterestWidget;
class UWorldmapPlayerPoiWidget;

class UOverlay;
class UImage;

/**
 * 
 */
UCLASS()
class G_API UWorldmapWidget : public UUserWidget
{
	GENERATED_BODY()

	FString current_map;
	
	UPROPERTY(meta = (BindWidget))
	UOverlay* map_overlay;

	UPROPERTY(meta = (BindWidget))
	UImage* map;

	float worldmap_dimension = 150000.0f;

	UPROPERTY()
	UWorldmapPointOfInterestWidget* player_icon;

	UPROPERTY(EditAnywhere)
	UTexture2D* player_icon_img;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> bp_player_icon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWorldmapPointOfInterestWidget> bp_poi_widget;

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* stage1_map;

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* cave_b1_map; 

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* cave_b2_map;

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* palace_map;
	
	void UpdatePlayerIcon(FVector2D& map_loc, float& zoom);
	void UpdateAllPoiIcon(FVector2D& map_loc, float& zoom);
	void UpdatePoiIcon(UUserWidget* poi_icon);
	
public:
	void Init();
	void On(FVector2D& map_loc, float& zoom);
	void Update(FVector2D& map_loc, float& zoom);
	void AddPoi(AActor* actor);
	void AddPlayerIcon(ECharacterName characater);
	void Off();
	void Set();

	void ChangeToStage1();

	UFUNCTION(BlueprintCallable)
	void ChangeToCaveB1();

	UFUNCTION(BlueprintCallable)
	void ChangeToCaveB2();
	
	void ChangeToPalace();
};
