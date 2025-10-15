// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CollocateWidget.generated.h"

class UUsableItemBase;
class USkillButton;
class USkillSlot;
class USkillButtonTooltip;
class UCollocatingSkillButton;
class APlayerCharacter;

class UButton;
class UHorizontalBox;
class UTexture2D;

/**
 * 
 */
UCLASS()
class G_API UCollocateWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SkillBox;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCollocatingSkillButton> BP_SkillButton;

	UPROPERTY()
	TArray<UCollocatingSkillButton*> ButtonArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillButtonTooltip> BP_SkillButtonTooltip;

	UPROPERTY()
	USkillButtonTooltip* SkillButtonTooltip;

	UFUNCTION()
	void Submit();

	UFUNCTION()
	void Cancel();

	UPROPERTY()
	UCollocatingSkillButton* CurrentCollocatedButton = nullptr;

	UPROPERTY()
	UUsableItemBase* Item = nullptr;

	UPROPERTY()
	APlayerCharacter* TargetPC = nullptr;

private:
	void AddSkillButton();
	
public:
	UFUNCTION()
	void Init();

	void On(TArray<USkillSlot*>& SkillSlotList, UUsableItemBase* _Item, APlayerCharacter* _TargetPC);
	void Off();

	void Set(TArray<USkillSlot*>& SkillSlotList, UUsableItemBase* _Item);
	/*void Set(USkillSlot* SkillSlot, int Index);*/
};
