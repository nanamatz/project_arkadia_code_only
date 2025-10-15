// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UOverlay;
class UBorder;
class UTextBlock;
class UItemBase;
class UActionMenuConsumables;
class UInventoryTooltip;

/**
 * 
 */
UCLASS()
class G_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UOverlay* overlay_slot;

	UPROPERTY(meta = (BindWidget))
	UBorder* icon_border;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* text_amount;

	UPROPERTY()
	UItemBase* item;

	UPROPERTY()
	UActionMenuConsumables* action_menu;

	UPROPERTY()
	UInventoryTooltip* tooltip;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* icon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActionMenuConsumables> bp_action_menu;

	void Init(UItemBase* item, UActionMenuConsumables* _action_menu, UInventoryTooltip* _tooltip);
	void Delete();
	void DeleteUIOnly();
	void Update();

	void SetEquip();
	void SetUnEquip();

	UItemBase* GetItem() { return item; };
};
