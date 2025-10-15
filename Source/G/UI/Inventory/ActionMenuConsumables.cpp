// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMenuConsumables.h"

#include "CollocateWidget.h"
#include "InventorySlotWidget.h"
#include "G/GGameInstance.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/Inventory/InventoryUI.h"
#include "G/G.h"
#include "G/PlayerCharacter.h"
#include "G/Item/UsableItemBase.h"
#include "G/Manager/CombatManager.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "G/UI/InGameUI/AnnotationWidget.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UActionMenuConsumables::Off()
{
	RemoveFromViewport();
}

//void UActionMenuConsumables::Init(UInventorySlotWidget* _inventory_slot, FString item_type)
void UActionMenuConsumables::Init()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UseButton->OnClicked.AddDynamic(this, &UActionMenuConsumables::UseButtonClicked);
	EquipButton->OnClicked.AddDynamic(this, &UActionMenuConsumables::EquipButtonClicked);
	UnequipButton->OnClicked.AddDynamic(this, &UActionMenuConsumables::UnequipButtonClicked);
	ThrowButton->OnClicked.AddDynamic(this, &UActionMenuConsumables::ThrowButtonClicked);
	CollocateButton->OnClicked.AddDynamic(this, &UActionMenuConsumables::CollocateButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UActionMenuConsumables::CancelButtonClicked);

	CollocateWidget = CreateWidget<UCollocateWidget>(this, BP_CollocateWidget);
	CollocateWidget->Init();
}

void UActionMenuConsumables::On(UInventorySlotWidget* _inventory_slot, UItemBase* item)
{
	inventory_slot = _inventory_slot;
	FString item_type = item->GetItemType();

	SetPositionInViewport(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()), false);

	SetVisibility(ESlateVisibility::Visible);

	AddToViewport(1);

	if (item_type.Equals("Equipment"))
	{
		UEquipItemBase* equip_item = Cast<UEquipItemBase>(item);
		
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
		CollocateButton->SetVisibility(ESlateVisibility::Collapsed);
		CancelButton->SetVisibility(ESlateVisibility::Visible);
		
		if(equip_item->GetEquipType() == EEquipType::weapon)
		{
			UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
			EquipButton->SetVisibility(ESlateVisibility::Collapsed);
			ThrowButton->SetVisibility(ESlateVisibility::Collapsed);

			return;
		}
		
		if (equip_item->IsEquip())
		{
			UnequipButton->SetVisibility(ESlateVisibility::Visible);
			EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
			EquipButton->SetVisibility(ESlateVisibility::Visible);
		}

		ThrowButton->SetVisibility(ESlateVisibility::Visible);
	}
	//만약 소모품이라면
	else if (item_type.Equals("Consumables"))
	{
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
		ThrowButton->SetVisibility(ESlateVisibility::Visible);
		CollocateButton->SetVisibility(ESlateVisibility::Visible);
		CancelButton->SetVisibility(ESlateVisibility::Visible);
	}
	//만약 재료라면
	else if (item_type.Equals("Material"))
	{
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
		ThrowButton->SetVisibility(ESlateVisibility::Visible);
		CollocateButton->SetVisibility(ESlateVisibility::Collapsed);
		CancelButton->SetVisibility(ESlateVisibility::Visible);
	}
	//만약 중요 아이템이라면
	else if (item_type.Equals("KeyItems") || item_type.Equals("Weapon"))
	{
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
		ThrowButton->SetVisibility(ESlateVisibility::Collapsed);
		CollocateButton->SetVisibility(ESlateVisibility::Visible);
		CancelButton->SetVisibility(ESlateVisibility::Visible);
	}
	//디폴트 값이 들어 있을 경우
	else if (item_type.Equals("Armor") || item_type.Equals("ItemBase"))
	{
		//exit(0);
	}
}

void UActionMenuConsumables::UseButtonClicked()
{
	if (inventory_slot)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UInventoryUI* InventoryUI = GameInstance->GetGameMenuManager()->GetInventoryUI();
		UUsableItemBase* item = Cast<UUsableItemBase>(inventory_slot->GetItem());

		item->Use(GameInstance->GetInGameDataManager()->GetPlayerCharacter(InventoryUI->GetTargetCharacter()));
		inventory_slot->Delete();
		RemoveFromViewport();
	}
}

void UActionMenuConsumables::EquipButtonClicked()
{
	if (inventory_slot)
	{
		UEquipItemBase* item = Cast<UEquipItemBase>(inventory_slot->GetItem());
		UInventoryUI* InventoryUI = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->
		                            GetGameMenuManager()->GetInventoryUI();
		
		if (item->Equip(InventoryUI->GetTargetCharacter()))
		{
			EquipButton->SetVisibility(ESlateVisibility::Collapsed);
			UnequipButton->SetVisibility(ESlateVisibility::Visible);

			inventory_slot->SetEquip();
		}
		else
		{
			/* fail to equip */
			LOG(Log, "Fail to Equip")
		}
	}
	else
	{
		LOG(Log, "Fail to Equip")
	}
}

void UActionMenuConsumables::UnequipButtonClicked()
{
	if (inventory_slot)
	{
		UEquipItemBase* item = Cast<UEquipItemBase>(inventory_slot->GetItem());

		item->Unequip();

		EquipButton->SetVisibility(ESlateVisibility::Visible);
		UnequipButton->SetVisibility(ESlateVisibility::Collapsed);

		inventory_slot->SetUnEquip();
	}
	else
	{
		LOG(Log, "Fail to UnEquip")
	}
}

void UActionMenuConsumables::ThrowButtonClicked()
{
	if (inventory_slot)
	{
		UEquipItemBase* equipment = Cast<UEquipItemBase>(inventory_slot->GetItem());

		if(equipment)
		{
			if(equipment->IsEquip())
			{
				UGGameInstance* GameInstance = Cast<UGGameInstance>(GetGameInstance());

				GameInstance->GetAnnotationWidget()->OnAnnotation(FText(LOCTEXT("InventoryIsEquip", "장착 중인 아이템입니다.")),3);
				return;
			}
		}
		
		inventory_slot->Delete();
		RemoveFromViewport();
	}
}

void UActionMenuConsumables::CollocateButtonClicked()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
\
	ECombatState CurCombatState = GameInstance->GetCombatManager()->GetCombatState();
	if (CurCombatState == CombatNone || CurCombatState == CombatEnd)
	{
		UInventoryUI* InventoryUI = GameInstance->GetGameMenuManager()->GetInventoryUI();
		UUsableItemBase* item = Cast<UUsableItemBase>(inventory_slot->GetItem());
		APlayerCharacter* TargetPC = GameInstance->GetInGameDataManager()->GetPlayerCharacter(
			InventoryUI->GetTargetCharacter());
		TArray<USkillSlot*>& SkillSlotList = TargetPC->GetSkillSystemComponent()->GetSkillSlotListRef();

		RemoveFromViewport();

		CollocateWidget->On(SkillSlotList, item, TargetPC);
	}
}

void UActionMenuConsumables::CancelButtonClicked()
{
	Off();
}

#undef LOCTEXT_NAMESPACE