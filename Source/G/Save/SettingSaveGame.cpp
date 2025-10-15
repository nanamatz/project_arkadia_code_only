// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingSaveGame.h"

#include "G/G.h"
#include "Kismet/GameplayStatics.h"

void USettingSaveGame::Save(FString slot_name, int32 idx, int32 _cam_mov_level, int32 _cam_rot_level, FText _language)
{
	cam_mov_level = _cam_mov_level;
	cam_rot_level = _cam_rot_level;
	language = _language;
	
	UGameplayStatics::SaveGameToSlot(this, slot_name, idx);
}

void USettingSaveGame::Save(FString slot_name, int32 idx, float master_volume, float effect_volume, float interaction_volume, float bg_volume)
{
	saved_master_volume = master_volume;
	saved_effect_volume = effect_volume;
	saved_interaction_volume = interaction_volume;
	saved_bg_volume = bg_volume;
	
	UGameplayStatics::SaveGameToSlot(this, slot_name, idx);
}
