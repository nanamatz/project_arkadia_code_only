// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCaptureWidget.h"
#include "Components/Image.h"
#include "G/Item/EquipItemBase.h"

void UCharacterCaptureWidget::Update(TMap<EEquipType, UEquipItemBase*>& equip_items)
{
	for(auto equip_item : equip_items)
	{
		UTexture2D* icon = equip_item.Value->GetIcon();

		switch (equip_item.Key)
		{
		case EEquipType::helmet:
			SetHelmet(icon);
			break;
		case EEquipType::chestplate:
			SetChestPlate(icon);
			break;
		case EEquipType::belt:
			SetBelt(icon);
			break;
		case EEquipType::amulet:
			SetAmulet(icon);
			break;
		case EEquipType::ring:
			SetRing(icon);
			break;
		case EEquipType::boots:
			SetBoots(icon);
			break;
		case EEquipType::gloves:
			SetGloves(icon);
			break;
		case EEquipType::weapon:
			SetWeapon(icon);
			break;
		}
	}
}

void UCharacterCaptureWidget::SetHelmet(UTexture2D* icon)
{
	helmet_img->SetBrushFromTexture(icon);
	helmet_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetChestPlate(UTexture2D* icon)
{
	chestplate_img->SetBrushFromTexture(icon);
	chestplate_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetBelt(UTexture2D* icon)
{
	belt_img->SetBrushFromTexture(icon);
	belt_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetAmulet(UTexture2D* icon)
{
	amulet_img->SetBrushFromTexture(icon);
	amulet_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetRing(UTexture2D* icon)
{
	ring_img->SetBrushFromTexture(icon);
	ring_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetBoots(UTexture2D* icon)
{
	boots_img->SetBrushFromTexture(icon);
	boots_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetGloves(UTexture2D* icon)
{
	gloves_img->SetBrushFromTexture(icon);
	gloves_img->SetOpacity(1);
}

void UCharacterCaptureWidget::SetWeapon(UTexture2D* icon)
{
	weapon_img->SetBrushFromTexture(icon);
	weapon_img->SetOpacity(1);
}

void UCharacterCaptureWidget::UnsetHelmet()
{
	helmet_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetChestPlate()
{
	chestplate_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetBelt()
{
	belt_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetAmulet()
{
	amulet_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetRing()
{
	ring_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetBoots()
{
	boots_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetGloves()
{
	gloves_img->SetOpacity(0);
}

void UCharacterCaptureWidget::UnsetWeapon()
{
	weapon_img->SetOpacity(0);
}
