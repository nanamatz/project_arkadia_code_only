// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

class UMaterialParameterCollection;
class UTextureRenderTarget2D;
class UTexture2D;
class UOverlay;
class UImage;

class UPlayerIconWidget;
class APlayerCharacter;
class UMinimapPointOfInterestWidget;
class UPointOfInterestWidget;

/**
 * 
 */
UCLASS()
class G_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

	/* variable */
	float dimensions = 150000;
	float zoom = 0.035f;
	float MapSize = 300.0f;
	FString current_map;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* map_data;

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D* render_target;

	/*UPROPERTY()
	UTexture2D* render_target_texture;*/

	UPROPERTY(meta = (BindeWidget))
	UOverlay* map_overlay;

	UPROPERTY(meta = (BindWidget))
	UImage* map;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> _player_icon;

	UPROPERTY()
	UPlayerIconWidget* player_icon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMinimapPointOfInterestWidget> BP_POIWidget;

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* stage1_map;

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* cave_b1_map; 

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* cave_b2_map;

	UPROPERTY(EditAnywhere, Category="MapImage")
	UTexture2D* palace_map;
	
	FVector player_location;
	FRotator player_rotation;

	UPROPERTY()
	APlayerCharacter* ControlCharacter;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* function */
public:
	void Init();

	float GetDimension();
	float GetZoom();
	float GetMapSize();

	APlayerCharacter* GetControlCharacter();

	void ChangeToStage1();

	UFUNCTION(BlueprintCallable)
	void ChangeToCaveB1();
	UFUNCTION(BlueprintCallable)
	void ChangeToCaveB2();
	void ChangeToPalace();
	
	void UpdateControlCharacter();
	void AddPOI(AActor* Actor);
	UPointOfInterestWidget* GetPOI(AActor* owner);
};
