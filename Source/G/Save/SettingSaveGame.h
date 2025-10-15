// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class G_API USettingSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 cam_mov_level;
	
	UPROPERTY()
	int32 cam_rot_level;

	UPROPERTY()
	FText language;
	
	UPROPERTY()
	float saved_master_volume;

	UPROPERTY()
	float saved_effect_volume;

	UPROPERTY()
	float saved_interaction_volume;

	UPROPERTY()
	float saved_bg_volume;
	
public:
	void Save(FString slot_name, int32 idx, int32 _cam_mov_level, int32 _cam_rot_level, FText _language);
	void Save(FString slot_name, int32 idx, float master_volume, float effect_volume, float interaction_volume, float bg_volume);

	int32 GetMoveLevel() { return cam_mov_level; }
	int32 GetRotLevel() { return cam_rot_level; }
	FText GetLanguage() { return language; }
	float GetMasterVolume() { return saved_master_volume; }
	float GetEffectVolume() { return saved_effect_volume; }
	float GetInteractionVolume() { return saved_interaction_volume; }
	float GetBgVolume() { return saved_bg_volume; }
};
