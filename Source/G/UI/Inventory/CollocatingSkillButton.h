// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/InGameUI/SkillButton.h"
#include "CollocatingSkillButton.generated.h"

class UTexture2D;
class UItemBase;
class UUsableItemBase;

/**
 * 
 */
UCLASS()
class G_API UCollocatingSkillButton : public USkillButton
{
	GENERATED_BODY()

	/*
	UCollocatingSkillButton** CurrentCollocatedButton = nullptr;

	UUsableItemBase** Item = nullptr;*/

public:
	void Init(USkillButtonTooltip* _tooltip_ptr, int32 _skill_num);

	/*UFUNCTION()
	void Collocate();*/

	void SetIcon(UTexture2D* icon);
};
