// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemPanelBase.h"
#include "AudioPanel.generated.h"

class USlider;
class USoundClass;

/**
 * 
 */
UCLASS()
class G_API UAudioPanel : public USystemPanelBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	USlider* master_sound_slider;
	
	UPROPERTY(meta = (BindWidget))
	USlider* effect_sound_slider;

	UPROPERTY(meta = (BindWidget))
	USlider* background_sound_slider;

	UPROPERTY(meta = (BindWidget))
	USlider* interaction_sound_slider;

	UPROPERTY(EditAnywhere)
	USoundClass* background_sc;

	UPROPERTY(EditAnywhere)
	USoundClass* effect_sc;

	UPROPERTY(EditAnywhere)
	USoundClass* interaction_sc;
	
protected:
	virtual bool Initialize() override;
	
private:
	void SetMasterVolume(float value);
	void SetEffectVolume(float value);
	void SetBackgroundVolume(float value);
	void SetInteractionVolume(float value);

	float GetMasterVolume();
	float GetEffectVolume();
	float GetBgVolume();
	float GetInteractionVolume();
	
	void Save();
	void Load();
	
public:
	virtual void On() override;
	virtual void Off() override;
	virtual void Apply() override;
};
