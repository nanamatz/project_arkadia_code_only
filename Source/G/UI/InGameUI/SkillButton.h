// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillButton.generated.h"

class UTexture2D;
class UTextBlock;
class UImage;
class UButton;
class USoundWave;

class USkillSlot;
class USkillButtonTooltip;

/**
 * 
 */
UCLASS()
class G_API USkillButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* cooldown_text;

	UPROPERTY(meta = (BindWidget))
	UImage* cooldown_img;

	UPROPERTY(EditAnywhere)
	UTexture2D* normal_skillButton;

	UPROPERTY(EditAnywhere)
	UTexture2D* pressed_skillButton;

	UPROPERTY(EditAnywhere)
	UTexture2D* hovered_skillButton;

	UPROPERTY()
	USkillSlot* skill_slot_ptr = nullptr;

	UPROPERTY()
	USkillButtonTooltip* tooltip_ptr = nullptr;

	UPROPERTY(EditAnywhere)
	USoundWave* button_clicked;

	int32 skill_num = 0;

	/*protected:
		virtual void NativeOnInitialized() override;*/

public:
	UPROPERTY(meta = (BindWidget))
	UButton* skill_button;

	UPROPERTY(meta = (BindWidget))
	UImage* skill_img;

protected:
	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnHovered();

private:
	UFUNCTION()
	void OnButtonClick_AttackSet();

public:
	virtual void Init(USkillButtonTooltip* _tooltip_ptr, int32 _skill_num);
	void Set(USkillSlot* skill_slot, bool& isPlayerTurn);
	void Clear();
	void SetCoolDown();
	void OffCoolDown();
};
