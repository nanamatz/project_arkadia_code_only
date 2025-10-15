// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemBase.h"

#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Util/Define.h"
#include  "G/Player/CharacterInfoStruct.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/Inventory/InventoryUI.h"
#include "G/UI/Inventory/StatBox.h"
#include "Kismet/GameplayStatics.h"

bool UEquipItemBase::Equip(ECharacterName name)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Equip: GameInstance Null"));

		return false;
	}

	FCharacterInfo& character_info = GameInstance->GetInGameDataManager()->GetPlayerCharacterInfo(name);

	auto val = character_info.equipped_item_map.Find(GetEquipType());

	if (!character_info.equipped_item_map.Contains(GetEquipType()))
	{
		EquipCharName = name;

		character_info.equipped_item_map.Add(GetEquipType(), this);
		character_info.Ability += ability;

		UInventoryUI* InventoryUI = GameInstance->GetGameMenuManager()->GetInventoryUI();

		InventoryUI->Equip(this->item_icon, GetEquipType(), name);

		if (name == InventoryUI->GetTargetCharacter())
		{
			InventoryUI->stat_box->Update(character_info.Ability);
		}

		return true;
	}

	return false;
}

void UEquipItemBase::Unequip()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FCharacterInfo& character_info = GameInstance->GetInGameDataManager()->GetPlayerCharacterInfo(EquipCharName);
	
	character_info.equipped_item_map.Remove(GetEquipType());
	character_info.Ability -= ability;

	UInventoryUI* InventoryUI = GameInstance->GetGameMenuManager()->GetInventoryUI();

	InventoryUI->Unequip(EquipCharName, GetEquipType());

	InventoryUI->stat_box->Update(character_info.Ability);

	EquipCharName = ECharacterName::None;
}

void UEquipItemBase::UnEquipUIOnly()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	UInventoryUI* InventoryUI = GameInstance->GetGameMenuManager()->GetInventoryUI();

	InventoryUI->Unequip(EquipCharName, GetEquipType());

	EquipCharName = ECharacterName::None;
}

bool UEquipItemBase::IsEquip()
{
	return EquipCharName != ECharacterName::None;
}

void UEquipItemBase::EnhenceDefault(int n)
{
	EnhenceValue += n;
}

void UEquipItemBase::Enhence()
{
	EnhenceValue++;
}
