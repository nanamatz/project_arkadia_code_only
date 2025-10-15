// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemPanelBase.h"
#include "GamePlayPanel.generated.h"

class UHorizontalComboBox;

/**
 * 
 */
UCLASS()
class G_API UGamePlayPanel : public USystemPanelBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* cam_mov_speed;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* cam_rot_speed;

	UPROPERTY(meta = (BindWidget))
	UHorizontalComboBox* language;

	UPROPERTY(EditAnywhere)
	TArray<float> cam_mov_speed_arr;
	
	UPROPERTY(EditAnywhere)
	TArray<float> cam_rot_speed_arr;

	void Save();
	void Load();
	
protected:
	virtual bool Initialize() override;
	
public:
	virtual void On() override;
	virtual void Off() override;
	virtual void Apply() override;
};
