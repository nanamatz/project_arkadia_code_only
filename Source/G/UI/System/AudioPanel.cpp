// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioPanel.h"

#include "AudioDevice.h"
#include "Components/Slider.h"
#include "G/G.h"
#include "G/Save/SettingSaveGame.h"
#include "Kismet/GameplayStatics.h"

class USettingSaveGame;

bool UAudioPanel::Initialize()
{
	bool res = Super::Initialize();

	Load();
	
	return res;
}

void UAudioPanel::SetMasterVolume(float value)
{
	if(master_sound_slider)
	{
		master_sound_slider->SetValue(value);
	}
}

void UAudioPanel::SetEffectVolume(float value)
{
	LOG(Log, "%f %f", value, background_sound_slider->GetValue());
	if(effect_sc)
		effect_sc->Properties.Volume = value * master_sound_slider->GetValue();

	if(effect_sound_slider)
		effect_sound_slider->SetValue(value);
}

void UAudioPanel::SetBackgroundVolume(float value)
{
	if(background_sc)
		background_sc->Properties.Volume = value * master_sound_slider->GetValue();

	if(background_sound_slider)
		background_sound_slider->SetValue(value);
}

void UAudioPanel::SetInteractionVolume(float value)
{
	if(interaction_sc)
		interaction_sc->Properties.Volume = value * master_sound_slider->GetValue();

	if(interaction_sound_slider)
		interaction_sound_slider->SetValue(value);
}

float UAudioPanel::GetMasterVolume()
{
	return master_sound_slider->GetValue();
}

float UAudioPanel::GetEffectVolume()
{
	if(effect_sc)
		return effect_sc->Properties.Volume;

	return 1;
}

float UAudioPanel::GetBgVolume()
{
	if(background_sc)
		return background_sc->Properties.Volume;

	return 1;
}

float UAudioPanel::GetInteractionVolume()
{
	if(interaction_sc)
		return interaction_sc->Properties.Volume;

	return 1;
}

void UAudioPanel::Save()
{
	FString slot_name = TEXT("SettingSaveSlot");
	int32 user_idx = 0;
	
	USettingSaveGame* save = Cast<USettingSaveGame>(UGameplayStatics::LoadGameFromSlot(slot_name, user_idx));

	if(!save)
	{
		save = Cast<USettingSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingSaveGame::StaticClass()));
	}
	
	save->Save(slot_name, user_idx, GetMasterVolume(), GetEffectVolume(), GetInteractionVolume(), GetBgVolume());
}

void UAudioPanel::Load()
{
	FString SlotName = TEXT("SettingSaveSlot");
	int32 UserIndex = 0;
	
	USettingSaveGame* save = Cast<USettingSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if(!IsValid(save))
		return;

	SetMasterVolume(save->GetMasterVolume());
	SetEffectVolume(save->GetEffectVolume());
	SetInteractionVolume(save->GetInteractionVolume());
	SetBackgroundVolume(save->GetBgVolume());;
}

void UAudioPanel::On()
{
	Super::On();
}

void UAudioPanel::Off()
{
	Super::Off();
}

void UAudioPanel::Apply()
{
	Super::Apply();

	float ms = master_sound_slider->GetValue();
	float es = effect_sound_slider->GetValue();
	float bs = background_sound_slider->GetValue();
	float is = interaction_sound_slider->GetValue();
	
	SetMasterVolume(ms);
	SetEffectVolume(es);
	SetBackgroundVolume(bs);
	SetInteractionVolume(is);

	Save();
}
