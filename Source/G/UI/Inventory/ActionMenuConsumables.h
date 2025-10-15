// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Item/ItemBase.h"
#include "ActionMenuConsumables.generated.h"

class UInventorySlotWidget;
class UCollocateWidget;

class USizeBox;
class UBorder;
class UVerticalBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class G_API UActionMenuConsumables : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UInventorySlotWidget* inventory_slot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCollocateWidget> BP_CollocateWidget;

	UPROPERTY()
	UCollocateWidget* CollocateWidget;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* OutSideSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* OutSideBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* OutVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UButton* UseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UseText;

	UPROPERTY(meta = (BindWidget))
	UButton* EquipButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EquipText;

	UPROPERTY(meta = (BindWidget))
	UButton* UnequipButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnequipText;

	UPROPERTY(meta = (BindWidget))
	UButton* ThrowButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ThrowText;

	UPROPERTY(meta = (BindWidget))
	UButton* CollocateButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CollocateText;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CancelText;

public:
	void Init();

	/*void On(class UInventorySlotWidget* _inventory_slot, FString item_type);*/
	void On(UInventorySlotWidget* _inventory_slot, UItemBase* item);

	void Off();

	UFUNCTION()
	void UseButtonClicked();

	UFUNCTION()
	void EquipButtonClicked();

	UFUNCTION()
	void UnequipButtonClicked();

	UFUNCTION()
	void ThrowButtonClicked();

	UFUNCTION()
	void CollocateButtonClicked();

	UFUNCTION()
	void CancelButtonClicked();
};
