// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootingUI.generated.h"

/**
 * 
 */
UCLASS()
class G_API ULootingUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "LootingUI Option")
	int32 max_slot_horizontal = 4;
	//const uint8_t max_slot_horizontal = 3;

	UPROPERTY(EditAnywhere, Category = "LootingUI Option")
	FVector2D DisplayInversionPoint = FVector2D(0.6f, 0.5f);

	UPROPERTY()
	class ULootingComponent* target_LTC;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* canvas_panel;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* LootingInventory;

	UPROPERTY(meta = (BindWidget))
	class UButton* AllGetButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULootingSlotWidget> bp_inventory_slot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UActionMenuConsumables> bp_action_menu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryTooltip> bp_inventory_tooltip;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY()
	class UInventoryTooltip* inventory_tooltip;

	void Insert(class UItemBase* item);
	bool Delete(int32 idx);
	void init();
	void SetItemList(class ULootingComponent* _target);
	class ULootingComponent* GetLootingComponent() { return target_LTC; }
	void OpenLootingUI(FVector2D pos);
	void LootingInventorySort();
	void InventoryEmptyCheck();

	UFUNCTION()
	void AllGet();

	UFUNCTION()
	void CloseUI();
};
