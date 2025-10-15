// Fill out your copyright notice in the Description page of Project Settings.
#include "InventorySlotWidget.h"
#include "ActionMenuConsumables.h"
#include <Kismet/GameplayStatics.h>

#include "InventoryTooltip.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Item/EquipItemBase.h"
#include "G/Manager/InventoryManager.h"

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetEffectingButton();

	if (key == EKeys::RightMouseButton)
	{
		action_menu->On(this, item);

		return reply;
	}

	return reply;
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	tooltip->On(item);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	tooltip->Off();
}

void UInventorySlotWidget::Init(UItemBase* _item, UActionMenuConsumables* _action_menu, UInventoryTooltip* _tooltip)
{
	SetIsFocusable(false);

	item = _item;

	action_menu = _action_menu;
	tooltip = _tooltip;

	FString item_type = item->GetItemType();
	icon->SetBrushFromTexture(item->GetIcon());
	//action_menu->Init(this, item_type);

	//만약 소모품, 재료, 중요 아이템이라면
	if (item_type.Equals("Consumables") || item_type.Equals("Material") || item_type.Equals("KeyItems"))
	{
		text_amount->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		text_amount->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlotWidget::Delete()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	LOG(Log, "")
	
	if (GameInstance->inventory->DeleteItem(item) == 0)
	{
		/* remove this widget */

		/*if (item->GetItemType() == "Equipment")
		{
			UEquipItemBase* equip_item = Cast<UEquipItemBase>(item);
			if (equip_item->IsEquip())
			{
				equip_item->Unequip();
			}
		}*/

		RemoveFromParent();
	}
}

void UInventorySlotWidget::DeleteUIOnly()
{
	RemoveFromParent();
	/*UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	LOG(Log, "");
	RemoveFromParent();
	if (GameInstance->inventory->DeleteItem(item) == 0)
	{
		/* remove this widget #1#

		/*
		if (item->GetItemType() == "Equipment")
		{
			UEquipItemBase* equip_item = Cast<UEquipItemBase>(item);
			if (equip_item->IsEquip())
			{
				equip_item->UnEquipUIOnly();
			}
		}
		#1#

		
	}*/
}

void UInventorySlotWidget::Update()
{
	if (item->count == 0)
	{
		this->Delete();
	}

	text_amount->SetText(FText::FromString(FString::FromInt(item->count)));
}

void UInventorySlotWidget::SetEquip()
{
	FLinearColor color(1, 0.5f, 0.5f);
	icon_border->SetBrushColor(color);
}

void UInventorySlotWidget::SetUnEquip()
{
	FLinearColor color(1, 1, 1);
	icon_border->SetBrushColor(color);
}
