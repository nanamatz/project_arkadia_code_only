// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySaveGame.h"
#include <Kismet/GameplayStatics.h>

#include "G/G.h"
#include "G/Item/EquipItemBase.h"
#include "G/Item/ItemBase.h"

void UInventorySaveGame::Save(FString slot_name, int32 user_idx, TArray<UItemBase*> item_list, int32& current_weight, int32& max_weight, int32& gold)
{
	for(auto item : item_list)
	{
		FSoftObjectPath retrieved_soft_object_path(item->GetClass()->GetPathName());
		UEquipItemBase* equipment_item = Cast<UEquipItemBase>(item);
		
		if(equipment_item)
		{
			saved_item_list.Add(retrieved_soft_object_path, {1, equipment_item->GetEnhenceLevel()});
		}
		else
		{
			saved_item_list.Add(retrieved_soft_object_path, {item->count, 0});
		}
	}

	saved_current_weigth = current_weight;
	saved_max_weight = max_weight;
	saved_gold = gold;
	
	UGameplayStatics::SaveGameToSlot(this, slot_name, user_idx);
}

TMap<FSoftObjectPath, FIntPair>& UInventorySaveGame::GetItemList()
{
	return saved_item_list;
}
