// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/GameMenuTooltip.h"
#include "InventoryTooltip.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class G_API UInventoryTooltip : public UGameMenuTooltip
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* item_name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* armor_power;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* enhance_value;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* strength_increment;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* insight_increment;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* swift_increment;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* relaxed_increment;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* careful_increment;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* luck_increment;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* weight_text;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* value_text;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* stat_increment_box;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void On(class UItemBase* item);
	void Off();
};
