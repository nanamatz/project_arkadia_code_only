// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "G/UI/GameMenuBase.h"
#include "MapUI.generated.h"

class APlayerCharacter;
class UPointOfInterestWidget;
class UWorldmapWidget;

class AActor;
class UTextureRenderTarget2D;
class UTexture2D;
class UMaterialInstance;
class UImage;

/**
 * 
 */
UCLASS()
class G_API UMapUI : public UGameMenuBase
{
	GENERATED_BODY()

	/* variable */
	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D* render_target;

	UPROPERTY()
	UTexture2D* render_target_texture;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* material;

	UPROPERTY(meta = (BindWidget))
	UImage* map_img;

	UPROPERTY(meta = (BindWidget))
	UWorldmapWidget* worldmap_widget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPointOfInterestWidget> BP_POIWidget;

	float camera_speed = 500.0f;;

	int32 width;
	int32 height;

	UPROPERTY()
	TArray<APlayerCharacter*> player_character_arr;

	UPROPERTY()
	APlayerCharacter* control_character;

	FVector player_location;
	FVector2D map_location;

	// expose on spawn check
	float dimensions = 5000;

	// expose on spawn check
	float zoom = 1.f;

	// instance editable check and expose on spawn check
	class Texture* image;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* map_data;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/*virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;*/

public:
	class AWorldSceneCapture2D* camera;

	/* function */
private:
	
public:
	virtual void On() override;
	virtual void Off() override;

	void Init();
	void CreateIcon();

	float GetDimension();
	float GetZoom();

	void SetZoom(float _zoom);

	UFUNCTION(BlueprintCallable)
	UWorldmapWidget* GetWorldmapWidget();
};
