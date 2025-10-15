// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingSlotWidget.h"
#include "G/UI/Inventory/ActionMenuConsumables.h"
#include <Kismet/GameplayStatics.h>

#include "G/UI/Inventory/InventoryTooltip.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "G/GGameInstance.h"
#include "G/Item/EquipItemBase.h"
#include "G/Manager/InventoryManager.h"
#include "G/UI/Looting/LootingUI.h"
#include "G/UI/Looting/LootingComponent.h"

void ULootingSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	tooltip->On(item);
}

void ULootingSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	tooltip->Off();
}

FReply ULootingSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetEffectingButton();

	if (key == EKeys::LeftMouseButton)
	{
		AddItem();
		return reply;
	}

	return reply;
}

void ULootingSlotWidget::init(UItemBase* _item, UActionMenuConsumables* _action_menu, UInventoryTooltip* _tooltip,
                              ULootingUI* lootingUI)
{
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	item = _item;

	action_menu = _action_menu;
	tooltip = _tooltip;

	FString item_type = item->GetItemType();

	OwnerLootingUI = lootingUI;

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

	text_amount->SetText(FText::FromString(FString::FromInt(item->count)));
}

void ULootingSlotWidget::AddItem(bool isSort)
{
	int32 count = item->count;
	item->count = 1;
	for (int i = 0; i < count; i++)
	{
		GameInstance->inventory->AddItem(item);
	}

	for (auto iter : OwnerLootingUI->GetLootingComponent()->ProvideItemInfoList)
	{
		if (iter.Key.GetDefaultObject()->Name.EqualTo(item->Name) && (iter.Value == count))
		{
			OwnerLootingUI->GetLootingComponent()->ProvideItemInfoList.Remove(iter.Key);
			UE_LOG(LogTemp, Log, TEXT("ULootingSlotWidget 91 :: List remove"));
			break;
		}
	}

	if (AddSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), AddSound);
	}

	RemoveFromParent();
	if (isSort)
	{
		OwnerLootingUI->LootingInventorySort();
	}
	if (OwnerLootingUI)
	{
		OwnerLootingUI->InventoryEmptyCheck();
	}
}

UItemBase* ULootingSlotWidget::GetItem()
{
	return item;
}
