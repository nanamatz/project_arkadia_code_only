// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "G/UI/Inventory/InventorySlotWidget.h"

void UItemBase::UpdateSlotUI()
{
	if (ui_slot)
	{
		ui_slot->Update();
	}
}

void UItemBase::SetSlotUI(UInventorySlotWidget* _ui_slot)
{
	ui_slot = _ui_slot;
}
