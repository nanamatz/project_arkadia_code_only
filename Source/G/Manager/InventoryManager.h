// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Manager/ManagerBase.h"
#include "InventoryManager.generated.h"

class UItemBase;
class UInventoryUI;
class UQuestManager;
class AGCharacter;
class UEquipItemBase;

/**
 * 
 */

USTRUCT()
struct FInventory
{
	GENERATED_BODY()

	/* inventory array */
	UPROPERTY()
	TArray<UItemBase*> item_list;

	/* current weight of bag */
	int32 current_weight = 0;

	/* maximum capacity the bag can hold */
	int32 max_weight = 100;

	int32 gold;
};

UCLASS()
class G_API UInventoryManager : public UManagerBase
{
	GENERATED_BODY()

	UPROPERTY()
	UInventoryUI* inventory_ui;

	UPROPERTY()
	UQuestManager* questmanager;

public:
	UPROPERTY()
	struct FInventory inventory;

	virtual void Init(UGGameInstance* gameInstance) override;

	void SetInventoryUI(UInventoryUI* _inventory_ui);
	void SetQuestManager(UQuestManager* _questamanager);
	void AddItem(UItemBase* item);
	void AddItemMultiple(UItemBase* item, int32 count = 1);
	
	int32 DeleteItem(UItemBase* item);

	/**
	 * @param item item ptr
	 * @param delete_cnt number of item to delete
	 * @return item->count >= delete_cnt, if item->count >= delete_cnt, then return true 
	 */
	bool DeleteItem(UItemBase* item, int32 delete_cnt);

	void AddGold(int32 gold);
	bool IsSlotEmpty(int32 index);
	UItemBase* GetItemBaseAtIndex(int32 index);
	bool CharacterEquipRelease(int32 index, FString equippart);
	void ItemEquip(AGCharacter* character, UEquipItemBase* item);

	int32 GetItemCount(FText ItemName);
	UItemBase* GetItemByName(FText ItemName);

	int32& GetCurrentWeightRef() { return inventory.current_weight; }
	int32& GetMaxWeightRef() { return inventory.max_weight; }
	int32& GetCurrentWeight() { return inventory.current_weight; }
	int32& GetMaxWeight() { return inventory.max_weight; }

	void ClearInventory();

	void Save(FString parent_dir, int32 user_idx = 0);
	void Load(FString parent_dir, int32 user_idx = 0);
};
