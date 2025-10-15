// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnQueuePortrait.generated.h"

class UBorder;
class UImage;
class UProgressBar;
class UCanvasPanel;
class URichTextBlock;
class UTexture2D;

class UTurnQueuePortraitTooltip;
class UBuffDebuffImage;
class AGCharacter;

/**
 * 
 */
UCLASS()
class G_API UTurnQueuePortrait : public UUserWidget
{
	GENERATED_BODY()

	const int8 max_buff_cnt_in_line = 5;

	UPROPERTY(meta = (BindWidget))
	UBorder* portrait_border;

	UPROPERTY(meta = (BindWidget))
	UImage* portrait;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* hp_bar;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* portrait_canvas;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* status_text;

	FAnchors buff_debuff_anchors = FAnchors(0, 4.f / 7.f, 0, 5.f / 7.f);

	UPROPERTY()
	UTurnQueuePortraitTooltip* tooltip;

	TArray<UBuffDebuffImage*> buff_debuff_arr;

public:
	UPROPERTY()
	AGCharacter* target_character;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply
	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void Init(AGCharacter* character, UTurnQueuePortraitTooltip* _tooltip);
	void SetBorder(UTexture2D& texture);
	void EmphasisCurrTurn();
	void EmphasisEmeny();
	void EmphasisCompanion();
	void DeEmphasis();
	void AddBuff(UTexture2D* texture, int32* turn_count);
	void AddDebuff(UTexture2D* texture, int32* turn_count);
	void UpdateHP(int32 current_hp, int32 max_hp, int32 armor);
	void ProcessTurn();
};
