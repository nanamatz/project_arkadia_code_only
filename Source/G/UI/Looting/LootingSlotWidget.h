// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootingSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class G_API ULootingSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	class UInventoryTooltip* tooltip;

	UPROPERTY()
	class UActionMenuConsumables* action_menu;

	UPROPERTY()
	class UItemBase* item;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* overlay_slot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_amount;

	UPROPERTY()
	class UGGameInstance* GameInstance;

	UPROPERTY()
	class ULootingUI* OwnerLootingUI;

	UPROPERTY(EditAnywhere)
	class USoundBase* AddSound;

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* icon;

	void init(class UItemBase* _item, class UActionMenuConsumables* _action_menu, class UInventoryTooltip* _tooltip,
	          class ULootingUI* lootingUI);
	//Add Item in Inventory
	void AddItem(bool isSort = true);
	//Returns a "item" variable of the UItemBase type 
	class UItemBase* GetItem();
};
