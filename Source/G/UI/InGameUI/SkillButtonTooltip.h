// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/InGameUI/InGameTooltip.h"
#include "SkillButtonTooltip.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class G_API USkillButtonTooltip : public UInGameTooltip
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UBorder* tooltip_border;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* cost;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* damage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* skill_name;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void On(class USkillSlot* mySkillSlot);
	void Off();
};
