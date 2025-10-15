// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCompanionToggleBox.h"
#include "InventoryCompanionToggleButton.h"
#include "G/PlayerCharacter.h"
#include "CharacterCaptureWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UInventoryCompanionToggleBox::AddToggleButton(APlayerCharacter* player_character, ECharacterName* target_character,
                                                   class UCharacterCaptureWidget* ccw)
{
	for (auto toggleButton : horizontal_box->GetAllChildren())
	{
		UInventoryCompanionToggleButton* curButton = Cast<UInventoryCompanionToggleButton>(toggleButton);
		if (curButton->GetTargetCharacterName() == player_character->GetECharacterName())
		{
			return;
		}
	}

	UInventoryCompanionToggleButton* new_toggle_button = WidgetTree->ConstructWidget<UInventoryCompanionToggleButton>(
		bp_inventory_toggle_button);


	new_toggle_button->Init(player_character, target_character, ccw, &cur_ccw);

	UHorizontalBoxSlot* hb_slot = horizontal_box->AddChildToHorizontalBox(new_toggle_button);

	hb_slot->Size = ESlateSizeRule::Fill;
	hb_slot->HorizontalAlignment = HAlign_Center;
	hb_slot->VerticalAlignment = VAlign_Center;

	FText player_character_name = player_character->GetCharacterName();
}

UCharacterCaptureWidget* UInventoryCompanionToggleBox::GetCurrentCaptureWidget() const
{
	return cur_ccw;
}

TArray<UWidget*> UInventoryCompanionToggleBox::GetChild()
{
	return horizontal_box->GetAllChildren();
}

void UInventoryCompanionToggleBox::Clear()
{
	int32 child_cnt = horizontal_box->GetChildrenCount();
	for (int i = 0; i < child_cnt; i++)
	{
		horizontal_box->RemoveChildAt(0);
	}
}
