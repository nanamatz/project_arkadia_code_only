// Fill out your copyright notice in the Description page of Project Settings.

#include "G/Manager/InventoryManager.h"
#include <Kismet/GameplayStatics.h>

#include "InGameDataManager.h"
#include "SaveManager.h"
#include "G/G.h"
#include "G/Item/ItemBase.h"
#include "G/Item/EquipItemBase.h"
#include "G/UI/Inventory/InventoryUI.h"
#include "G/GCharacter.h"
#include "G/Character/CharacterInfo.h"
#include "G/GGameInstance.h"
#include "G/PlayerCharacter.h"
#include "G/Dialog/FQuestManager.h"
#include "G/Item/WeaponItemBase.h"
#include "G/Save/InventorySaveGame.h"
#include "G/UI/InGameUI/AnnotationWidget.h"

#define LOCTEXT_NAMESPACE "InventoryManager"

void UInventoryManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

void UInventoryManager::SetInventoryUI(UInventoryUI* _inventory_ui)
{
	inventory_ui = _inventory_ui;
}

void UInventoryManager::SetQuestManager(UQuestManager* _questamanager)
{
	questmanager = _questamanager;
}

void UInventoryManager::AddItem(UItemBase* item)
{
	if (item != nullptr)
	{
		if (inventory.max_weight >= inventory.current_weight + item->weight)
		{
			if (!item->GetItemType().Equals("Equipment"))
			{
				/* whether the item exist or not  */

				for (auto _item : inventory.item_list)
				{
					if (_item->Name.EqualTo(item->Name))
					{
						_item->count++;
						inventory.current_weight += item->weight;
						_item->UpdateSlotUI();
						inventory_ui->SetWeight(inventory.current_weight, inventory.max_weight);

						questmanager->ChangeGoalItemCount(item);

						return;
					}
				}
			}

			inventory.item_list.Add(item);
			inventory.current_weight += item->weight;
			inventory_ui->Insert(item, inventory.current_weight, inventory.max_weight);

			if (!GameInstance)
			{
				UE_LOG(LogTemp, Log, TEXT("GameInstance is null"));
			}
			else if (!GameInstance->QuestManager)
			{
				UE_LOG(LogTemp, Log, TEXT("GameInstance->QeustManager is null"));
			}
			questmanager->ChangeGoalItemCount(item);
		}
		else
		{
			/* print UI that notice the bag is full*/
			GameInstance->GetAnnotationWidget()->OnAnnotation(LOCTEXT("InventoryManager_Full", "가방이 가득 찼습니다."), 3);
		}
	}
}

void UInventoryManager::AddItemMultiple(UItemBase* item, int32 count)
{
}

int32 UInventoryManager::DeleteItem(UItemBase* item)
{
	LOG(Log, "");
	item->count--;

	if (item->count >= 1)
	{
		inventory.current_weight -= item->weight;
		inventory_ui->SetWeight(inventory.current_weight, inventory.max_weight);
		item->UpdateSlotUI();
		questmanager->ChangeGoalItemCount(item);
		return item->count;
	}
	if (item->count == 0)
	{
		inventory.current_weight -= item->weight;
		inventory_ui->SetWeight(inventory.current_weight, inventory.max_weight);
		item->UpdateSlotUI();
		inventory.item_list.Remove(item);
		questmanager->ChangeGoalItemCount(item);
		return 0;
	}

	return 0;
}

bool UInventoryManager::DeleteItem(UItemBase* item, int32 delete_cnt)
{
	if (item->count >= delete_cnt)
	{
		inventory.current_weight -= item->weight * delete_cnt;
		inventory_ui->SetWeight(inventory.current_weight, inventory.max_weight);
		item->count -= delete_cnt;
		item->UpdateSlotUI();

		if (item->count == 0)
		{
			inventory.item_list.Remove(item);
		}
		questmanager->ChangeGoalItemCount(item);
		return true;
	}

	return false;
}

void UInventoryManager::AddGold(int32 gold)
{
	inventory.gold += gold;
	inventory_ui->SetGold(inventory.gold);
}

bool UInventoryManager::IsSlotEmpty(int32 index)
{
	if (index < inventory.item_list.Num())
	{
		return false;
	}
	return true;
}

UItemBase* UInventoryManager::GetItemBaseAtIndex(int32 index)
{
	if (index < inventory.item_list.Num())
	{
		return inventory.item_list[index];
	}
	return nullptr;
}

bool UInventoryManager::CharacterEquipRelease(int32 index, FString equippart)
{

	return false;
}

void UInventoryManager::ItemEquip(AGCharacter* character, UEquipItemBase* item)
{
	auto equipped_item = character->GetCharacterInfo().equipped_item_map.Find(item->GetEquipType());

	if (equipped_item)
	{
		LOG(Log, "Unequip")
		(*equipped_item)->Unequip();
	}

	LOG(Log, "Equip")
	item->Equip(character->GetECharacterName());
}

int32 UInventoryManager::GetItemCount(FText ItemName)
{
	int32 AmountResult = 0;

	for (auto index : inventory.item_list)
	{
		if (ItemName.EqualTo(index->Name))
		{
			AmountResult += index->count;
		}
	}
	return AmountResult;
}

UItemBase* UInventoryManager::GetItemByName(FText ItemName)
{
	for (auto index : inventory.item_list)
	{
		if (ItemName.EqualTo(index->Name))
		{
			return index;
		}
	}
	return nullptr;
}

void UInventoryManager::ClearInventory()
{
	inventory_ui->ClearInventorySlot();

	inventory.gold = 0;
	inventory.current_weight = 0;
	inventory.max_weight = 100;

	inventory.item_list.Empty();
	
	if (questmanager)
	{
		questmanager->AllItemGoalQuestSetOnGoing();
	}
}

void UInventoryManager::Save(FString parent_dir, int32 user_idx)
{
	FString slot_name = parent_dir + TEXT("/InventorySaveSlot");
	
	UInventorySaveGame* save = Cast<UInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(slot_name, user_idx));

	if(!save)
	{
		save = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UInventorySaveGame::StaticClass()));
	}
	
	save->Save(slot_name, user_idx, inventory.item_list, inventory.current_weight, inventory.max_weight, inventory.gold);
}

void UInventoryManager::Load(FString parent_dir, int32 user_idx)
{
	FString slot_name = parent_dir + TEXT("/InventorySaveSlot");
	
	UInventorySaveGame* save = Cast<UInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(slot_name, user_idx));
	
	if(!IsValid(save))
		return;
	
	TMap<FSoftObjectPath, FIntPair> saved_item_list = save->GetItemList();
	for(TTuple<FSoftObjectPath, FIntPair> item_tuple : saved_item_list)
	{
		UClass* LoadedClass = Cast<UClass>(item_tuple.Key.TryLoad());
		if(LoadedClass)
		{
			UItemBase* loaded_item = NewObject<UItemBase>(this, LoadedClass);			
			
			if(IsValid(loaded_item))
			{
				UEquipItemBase* equipment = Cast<UEquipItemBase>(loaded_item);
				if(equipment)
				{
					equipment->EnhenceDefault(item_tuple.Value.enhance);
				}
				
				if(loaded_item->GetItemType().Equals("Equipment"))
				{
					UWeaponItemBase* weapon = Cast<UWeaponItemBase>(loaded_item);

					if(IsValid(weapon))
					{
						if(weapon->GetEquipCharName() != ECharacterName::None)
						{
							GameInstance->GetInGameDataManager()->GetPlayerCharacter(weapon->GetEquipCharName())->GetBoundWeapon()->EnhenceDefault(item_tuple.Value.enhance);
						}
						
						continue;
					}
				}
				for(int i = 0; i < item_tuple.Value.count; i++)
					AddItem(loaded_item);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE