// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemPanelBase.h"
#include "GraphicPanel.generated.h"

class UHorizontalComboBox;

/**
 * 
 */
UCLASS()
class G_API UGraphicPanel : public USystemPanelBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* window_mode;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* resolution;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* fps_limit;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* view_distance;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* post_processing;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* anti_aliasing;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* texture_quality;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* shadow_quality;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* shading_quality;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* effect_quality;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* foliage_quality;
	
protected:
	virtual bool Initialize() override;

	/* function */
private:
	EWindowMode::Type ConvertWindowMode(FText mode);
	FText ConvertWindowMode(EWindowMode::Type mode);
	float ConvertFPS(FText fps);
	FText ConvertFPS(float fps);
	FIntPoint ConvertResolution(FText _resolution);

public:
	virtual void On() override;
	virtual void Off() override;
	virtual void Apply() override;

	FIntPoint GetResolution();
};
