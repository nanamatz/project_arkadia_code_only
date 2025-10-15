// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Inventory/InventoryTooltip.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "G/Item/ItemBase.h"
#include "G/Item/ArmorItemBase.h"
#include "G/Item/EquipItemBase.h"
#include "G/Item/WeaponItemBase.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UInventoryTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	FVector2D viewport_center = UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / 2.0f;
	FVector2D mouse_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	if(mouse_pos.X > viewport_center.X)
	{
		mouse_pos.X -= GetDesiredSize().X + 10;
	} else
	{
		mouse_pos.X += 20;
	}

	if(mouse_pos.Y > viewport_center.Y)
	{
		mouse_pos.Y -= GetDesiredSize().Y + 10;
	} else
	{
		mouse_pos.Y += 10;
	}
	
	SetPositionInViewport(mouse_pos, false);
}

void UInventoryTooltip::On(UItemBase* item)
{
	bHasScriptImplementedTick = true;
	if (item)
	{
		FVector2D viewport_center = UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / 2.0f;
		FVector2D mouse_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

		if(mouse_pos.X > viewport_center.X)
		{
			mouse_pos.X -= GetDesiredSize().X + 10;
		} else
		{
			mouse_pos.X += 20;
		}

		if(mouse_pos.Y > viewport_center.Y)
		{
			mouse_pos.Y -= GetDesiredSize().Y + 10;
		} else
		{
			mouse_pos.Y += 10;
		}
	
		SetPositionInViewport(mouse_pos, false);
		
		item_name->SetText(FText::Format(LOCTEXT("InventoryTooltipItemName", "{0}"), item->Name));
		description->SetText(FText::Format(LOCTEXT("InventoryTooltipDesc", "{0}"), item->Description));
		enhance_value->SetVisibility(ESlateVisibility::Collapsed);
		armor_power->SetVisibility(ESlateVisibility::Collapsed);
		
		if (item->GetItemType().Equals("Equipment"))
		{			
			UEquipItemBase* equipment_item = Cast<UEquipItemBase>(item);

			enhance_value->SetVisibility(ESlateVisibility::Visible);
			armor_power->SetVisibility(ESlateVisibility::Visible);
			
			if (equipment_item->GetEquipType() == EEquipType::weapon)
			{
				UWeaponItemBase* weapon_item = Cast<UWeaponItemBase>(equipment_item);
				armor_power->SetText(
					FText::Format(LOCTEXT("InventoryTooltipPower", "power: +{0}"), weapon_item->GetPower()));
			}
			else
			{
				UArmorItemBase* armor_item = Cast<UArmorItemBase>(equipment_item);
				armor_power->SetText(
					FText::Format(LOCTEXT("InventoryTooltipArmor", "armor: +{0}"), armor_item->GetArmor()));
			}

			enhance_value->SetText(FText::Format(LOCTEXT("InventoryTooltipEnhance", "enhance: +{0}"), equipment_item->GetEnhenceLevel()));
			strength_increment->SetText(FText::Format(
				LOCTEXT("InventoryTooltipStr", "strength: +{0}"), equipment_item->ability.Value[EAbility::Strength]));
			insight_increment->SetText(FText::Format(
				LOCTEXT("InventoryTooltipInt", "insight: +{0}"), equipment_item->ability.Value[EAbility::Insight]));
			swift_increment->SetText(FText::Format(
				LOCTEXT("InventoryTooltipSwi", "swift: +{0}"), equipment_item->ability.Value[EAbility::Swift]));
			relaxed_increment->SetText(FText::Format(
				LOCTEXT("InventoryTooltipRel", "relaxed: +{0}"), equipment_item->ability.Value[EAbility::Relaxed]));
			careful_increment->SetText(FText::Format(
				LOCTEXT("InventoryTooltipCar", "careful: +{0}"), equipment_item->ability.Value[EAbility::Careful]));
			luck_increment->SetText(FText::Format(
				LOCTEXT("InventoryTooltipLuk", "luck: +{0}"), equipment_item->ability.Value[EAbility::Luck]));

			stat_increment_box->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			armor_power->SetVisibility(ESlateVisibility::Collapsed);
			enhance_value->SetVisibility(ESlateVisibility::Collapsed);
			stat_increment_box->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		weight_text->SetText(FText::Format(LOCTEXT("InventoryTooltipWeight", "{0}kg"), item->weight));
		value_text->SetText(FText::Format(LOCTEXT("InventoryTooltipValue", "value: {0}"), item->value));

		AddToViewport();
	}
}

void UInventoryTooltip::Off()
{
	bHasScriptImplementedTick = false;
	RemoveFromViewport();
}

#undef LOCTEXT_NAMESPACE
