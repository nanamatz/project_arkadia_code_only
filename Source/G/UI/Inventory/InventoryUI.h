// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/GameMenuBase.h"
#include "G/Util/Define.h"
#include "InventoryUI.generated.h"

class UCanvasPanel;
class UOverlay;
class UScrollBox;
class UProgressBar;
class UTextBlock;
class UExpBox;

class UItemBase;
class UStatBox;
class UInventoryCompanionToggleBox;
class UInventorySlotWidget;
class UActionMenuConsumables;
class UInventoryTooltip;
class UCharacterCaptureWidget;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class G_API UInventoryUI : public UGameMenuBase
{
	GENERATED_BODY()

	/* variable */
	const uint8_t max_slot_horizontal = 8;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* canvas_panel;

	/* character figure */
	UPROPERTY(meta = (BindWidget))
	UOverlay* character_capture_slot;

	/* inventory */
	UPROPERTY(meta = (BindWidget))
	UScrollBox* inventory;

	/* inventory weight */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* weight_bar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* weight_text;

	/* gold */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* gold_text;

	UPROPERTY(meta = (BindWidget))
	UInventoryCompanionToggleBox* companion_toggle_box;

	UPROPERTY(EditAnywhere)
	UTexture2D* default_slot_border;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlotWidget> bp_inventory_slot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActionMenuConsumables> bp_action_menu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryTooltip> bp_inventory_tooltip;

	ECharacterName target_character;

	TMap<ECharacterName, UCharacterCaptureWidget*> character_capture_widget_map;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(meta = (BindWidget))
	UStatBox* stat_box;

	UPROPERTY(meta = (BindWidget))
	UExpBox* exp_box;

	UPROPERTY()
	UActionMenuConsumables* action_menu;

	UPROPERTY()
	UInventoryTooltip* inventory_tooltip;

	/* function */
private:

public:
	virtual void On() override;
	virtual void Off() override;

	void Init(int32& current_weight, int32& max_weight);
	void Insert(UItemBase* item, int32& current_weight, int32& max_weight) const;
	void AddCharacter(APlayerCharacter* player_character);
	bool Delete(int32 idx) const;
	
	void SetGold(int32 gold) const;
	void SetWeight(int32 current_w, int32 max_w) const;
	void SetExpBox(int32& level, float percent) const;
	void SetExp(float percent) const;
	void SetLevel(int32& level) const;
	
	void AddCharacterCaptureWidget(UCharacterCaptureWidget* ccw) const;
	
	void Equip(UTexture2D* icon, enum class EEquipType equip_type) const;
	void Equip(UTexture2D* icon, enum class EEquipType equip_type, ECharacterName name) const;
	void Unequip(enum ECharacterName& name, enum class EEquipType equip_type);
	
	void ClearCaptureSlotAndCTB();
	void ClearInventorySlot() const;
	void ClearInventorySlotUIOnly() const;
	void ClearCaptureSlot();
	void ClearCompanionToggleBox();

	ECharacterName GetTargetCharacter() const { return target_character; }
};
