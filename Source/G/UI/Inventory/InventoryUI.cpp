// Fill out your copyright notice in the Description page of Project Settings.
#include "InventoryUI.h"

#include <G/Manager/TutorialManager.h>

#include "InventorySlotWidget.h"
#include "ActionMenuConsumables.h"
#include "InventoryCompanionToggleBox.h"
#include "CharacterCaptureWidget.h"
#include "ExpBox.h"
#include "InventoryCompanionToggleButton.h"
#include "InventoryTooltip.h"
#include "G/Manager/InventoryManager.h"
#include "StatBox.h"

#include "G/PlayerCharacter.h"
#include "G/GGameInstance.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Item/ItemBase.h"
#include "G/Item/EquipItemBase.h"
#include "G/UI/GameMenuManager.h"

#include "Blueprint/WidgetTree.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScrollBoxSlot.h"
#include "G/G.h"
#include "G/Item/WeaponItemBase.h"
#include "G/UI/System/GraphicPanel.h"
#include "G/UI/System/SystemUI.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "Arkadia"

FReply UInventoryUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey key = InKeyEvent.GetKey();

	if (key == EKeys::I || key == EKeys::Escape)
	{
		UGameMenuManager* gm = Cast<UGameMenuManager>(GetOuter());

		if(gm)
		{
			LOG(Log, "gm valid");
			gm->Off();
		} else
		{
			UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->GetGameMenuManager()->Off();			
		}

		return FReply::Handled();
	}
	if (key == EKeys::K || key == EKeys::M || key == EKeys::Q)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		GameInstance->GetGameMenuManager()->ChangeSubUI(key);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UInventoryUI::On()
{
	Super::On();

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GameInstance->GetTutorialManager()->PlayTutorial(ETutorial::Inventory);
	
	if(GameInstance)
	{
		FIntPoint resolution = GameInstance->GetSystemUI()->GetGraphicPanel()->GetResolution();
	}
}

void UInventoryUI::Off()
{
	Super::Off();

	action_menu->Off();
	inventory_tooltip->Off();
}

void UInventoryUI::Init(int32& current_weight, int32& max_weight)
{
	weight_bar->SetPercent(static_cast<float>(current_weight) / max_weight);
	weight_text->SetText(FText::Format(NSLOCTEXT("ENG", "weight", "{0} / {1}"), current_weight, max_weight));
	action_menu = CreateWidget<UActionMenuConsumables>(this, bp_action_menu);
	action_menu->Init();
	inventory_tooltip = CreateWidget<UInventoryTooltip>(this, bp_inventory_tooltip);
}

/*
* Add a New Item to the inventory
* class UTexture2D *item_icon :
*/
void UInventoryUI::Insert(class UItemBase* item, int32& current_weight, int32& max_weight) const
{
	/* Get last horizontal box */
	UHorizontalBox* last_horizontal_box = Cast<
		UHorizontalBox>(inventory->GetChildAt(inventory->GetChildrenCount() - 1));

	///* if you need to create a new horizontal box, then create one */
	if (!last_horizontal_box || last_horizontal_box->GetChildrenCount() >= max_slot_horizontal)
	{
		UHorizontalBox* new_horizontal_box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		UScrollBoxSlot* sb_slot = Cast<UScrollBoxSlot>(inventory->AddChild(new_horizontal_box));
		sb_slot->SetHorizontalAlignment(HAlign_Left);
		sb_slot->SetVerticalAlignment(VAlign_Fill);

		last_horizontal_box = new_horizontal_box;
	}

	/* create slot border */
	UInventorySlotWidget* new_inventory_slot = CreateWidget<UInventorySlotWidget>(GetWorld(), bp_inventory_slot);

	new_inventory_slot->Init(item, action_menu, inventory_tooltip);
	item->SetSlotUI(new_inventory_slot);

	/* add slot_border to the child of last_horizontal_box */
	UHorizontalBoxSlot* hb_slot = last_horizontal_box->AddChildToHorizontalBox(new_inventory_slot);
	hb_slot->SetSize(ESlateSizeRule::Automatic);

	weight_bar->SetPercent(static_cast<float>(current_weight) / max_weight);
	weight_text->SetText(FText::Format(NSLOCTEXT("ENG", "weight", "{0} / {1}"), current_weight, max_weight));
}

/// <summary>
/// Update the New Character Info
/// </summary>
void UInventoryUI::AddCharacter(APlayerCharacter* player_character)
{
	if(!character_capture_widget_map.Contains(player_character->GetECharacterName()))
	{
	    LOG(Log, "");
		UCharacterCaptureWidget* ccw = CreateWidget<UCharacterCaptureWidget>(
		this, player_character->bp_character_capture_widget);

		ccw->Update(player_character->GetCharacterInfo().equipped_item_map);
		
		UOverlaySlot* os = character_capture_slot->AddChildToOverlay(ccw);

		character_capture_widget_map.Add(player_character->GetECharacterName(), ccw);
		companion_toggle_box->AddToggleButton(player_character, &target_character, ccw);

		os->SetHorizontalAlignment(HAlign_Fill);
		os->SetVerticalAlignment(VAlign_Fill);
		
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		UEquipItemBase** Item = GameInstance->GetInGameDataManager()->GetPlayerCharacterInfo(player_character->GetECharacterName()).equipped_item_map.Find(player_character->GetBoundWeapon()->GetEquipType());
		
		if(!Item)
		{
			LOG(Log, "Add Weapon")
			GameInstance->GetInventoryManager()->AddItem(player_character->GetBoundWeapon());
			GameInstance->GetInventoryManager()->ItemEquip(player_character, player_character->GetBoundWeapon());
		}
	}
	
	auto characterInfoManager = player_character->GetCharacterInfoManager();
	
	stat_box->Update(characterInfoManager->GetAbilityRef());

	/* 0.1f -> character_info->cur_exp / character_info->max_exp */
	int32 characterLevel = characterInfoManager->GetLevel();
	exp_box->Update(characterLevel, characterInfoManager->GetExpPercent());
	
}

/*
* Delete the item matching the index
* int32 idx : range is 0 to (max_slot_horizontal - 1)
*/
bool UInventoryUI::Delete(int32 idx) const
{
	/* get horizontal box in which the target item is placed */
	UHorizontalBox* horizontal_box = Cast<UHorizontalBox>(inventory->GetChildAt(idx / max_slot_horizontal));

	/* if RemoveChild failed, then return false */
	if (!horizontal_box->RemoveChildAt(idx % max_slot_horizontal))
	{
		return false;
	}

	return true;
}

void UInventoryUI::SetWeight(int32 current_w, int32 max_w) const
{
	weight_bar->SetPercent(static_cast<float>(current_w) / static_cast<float>(max_w));
	weight_text->SetText(FText::Format(NSLOCTEXT("ENG", "weight", "{0} / {1}"), current_w, max_w));
}

void UInventoryUI::SetExpBox(int32& level, float percent) const
{
	exp_box->Update(level, percent);
}

void UInventoryUI::SetExp(float percent) const
{
	exp_box->SetExp(percent);
}

void UInventoryUI::SetLevel(int32& level) const
{
	exp_box->SetLevel(level);
}

void UInventoryUI::SetGold(int32 gold) const
{
	gold_text->SetText(FText::FromString(FString::FromInt(gold)));
}

void UInventoryUI::AddCharacterCaptureWidget(UCharacterCaptureWidget* ccw) const
{
	character_capture_slot->AddChildToOverlay(ccw);
	LOG(Log, "ccs: %d", character_capture_slot->GetChildrenCount());
}

void UInventoryUI::Equip(UTexture2D* icon, EEquipType equip_type) const
{
	UCharacterCaptureWidget* ccw = companion_toggle_box->GetCurrentCaptureWidget();

	switch (equip_type)
	{
	case EEquipType::helmet:
		ccw->SetHelmet(icon);
		break;
	case EEquipType::chestplate:
		ccw->SetChestPlate(icon);
		break;
	case EEquipType::belt:
		ccw->SetBelt(icon);
		break;
	case EEquipType::amulet:
		ccw->SetAmulet(icon);
		break;
	case EEquipType::ring:
		ccw->SetRing(icon);
		break;
	case EEquipType::boots:
		ccw->SetBoots(icon);
		break;
	case EEquipType::gloves:
		ccw->SetGloves(icon);
		break;
	case EEquipType::weapon:
		ccw->SetWeapon(icon);
		break;
	default:
		break;
	}
}

void UInventoryUI::Equip(UTexture2D* icon, EEquipType equip_type, ECharacterName name) const
{
	for(auto ctb: companion_toggle_box->GetChild())
	{
		UInventoryCompanionToggleButton* _ctb = Cast<UInventoryCompanionToggleButton>(ctb);

		if(IsValid(_ctb))
		{
			if(_ctb->GetTargetCharacterName() == name)
			{
				_ctb->SetInventoryTarget();
				Equip(icon, equip_type);

				return;
			}
		}
	}
}

void UInventoryUI::Unequip(ECharacterName& name, EEquipType equip_type)
{
	UCharacterCaptureWidget* ccw = character_capture_widget_map[name];

	switch (equip_type)
	{
	case EEquipType::helmet:
		ccw->UnsetHelmet();
		break;
	case EEquipType::chestplate:
		ccw->UnsetChestPlate();
		break;
	case EEquipType::belt:
		ccw->UnsetBelt();
		break;
	case EEquipType::amulet:
		ccw->UnsetAmulet();
		break;
	case EEquipType::ring:
		ccw->UnsetRing();
		break;
	case EEquipType::boots:
		ccw->UnsetBoots();
		break;
	case EEquipType::gloves:
		ccw->UnsetGloves();
		break;
	case EEquipType::weapon:
		ccw->UnsetWeapon();
		break;
	default:
		break;
	}
}

void UInventoryUI::ClearCaptureSlotAndCTB()
{
	ClearCaptureSlot();
	ClearCompanionToggleBox();
}

void UInventoryUI::ClearInventorySlot() const
{
	for (UWidget* _inventory_hb : inventory->GetAllChildren())
	{
		UHorizontalBox* inventory_hb = Cast<UHorizontalBox>(_inventory_hb);
		if (inventory_hb)
		{
			for (auto _inventory_slot : inventory_hb->GetAllChildren())
			{
				UInventorySlotWidget* inventory_slot = Cast<UInventorySlotWidget>(_inventory_slot);
				inventory_slot->DeleteUIOnly();
			}
		}
	}
}

void UInventoryUI::ClearInventorySlotUIOnly() const
{
	for (UWidget* _inventory_hb : inventory->GetAllChildren())
	{
		UHorizontalBox* inventory_hb = Cast<UHorizontalBox>(_inventory_hb);
		if (inventory_hb)
		{
			for (auto _inventory_slot : inventory_hb->GetAllChildren())
			{
				UInventorySlotWidget* inventory_slot = Cast<UInventorySlotWidget>(_inventory_slot);
				inventory_slot->DeleteUIOnly();
			}
		}
	}
}

void UInventoryUI::ClearCaptureSlot()
{
	int32 capture_slot_cnt = character_capture_slot->GetChildrenCount();
	for (int i = 0; i < capture_slot_cnt; i++)
	{
		character_capture_slot->RemoveChildAt(0);
	}
	
	character_capture_widget_map.Empty();
}

void UInventoryUI::ClearCompanionToggleBox()
{
	companion_toggle_box->Clear();
}

#undef LOCTEXT_NAMESPACE