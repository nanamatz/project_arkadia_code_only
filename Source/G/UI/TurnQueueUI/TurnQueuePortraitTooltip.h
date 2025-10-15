// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/InGameUI/InGameTooltip.h"
#include "G/Player/CharacterInfoStruct.h"
#include "TurnQueuePortraitTooltip.generated.h"

class UTextBlock;
class UProgressBar;
class UScrollBox;

class UTurnQueuePortraitTooltipStatus;
class AGCharacter;

/**
 * 
 */
UCLASS()
class G_API UTurnQueuePortraitTooltip : public UInGameTooltip
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UTextBlock* name_text;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* hp_bar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* hp;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* status_box;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTurnQueuePortraitTooltipStatus> bp_status;

	UPROPERTY()
	AGCharacter* cur_target_character = nullptr;

	UPROPERTY()
	TMap<EBUFFTYPE, UTexture2D*> buff_texture_map;

	UPROPERTY()
	TMap<EDEBUFFTYPE, UTexture2D*> debuff_texture_map;

	UPROPERTY()
	TArray<UTurnQueuePortraitTooltipStatus*> status_pool;

	/* func */
	void UpdateBuffDeBuff();

public:
	void Init(TMap<EBUFFTYPE, UTexture2D*>& _buff_texture_map, TMap<EDEBUFFTYPE, UTexture2D*>& _debuff_texture_map);
	void Update();
	void On(AGCharacter* target_character);
	void Off();

	FText BuffTypeToFText(EBUFFTYPE& buff_type);
	FText DeBuffTypeToFText(EDEBUFFTYPE& debuff_type);

	AGCharacter* GetTargetCharcter();
};
