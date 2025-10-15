// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InventorySaveGame.generated.h"

class UItemBase;

/**
 * 
 */

USTRUCT()
struct FIntPair
{
	GENERATED_BODY()

	UPROPERTY()
	int32 count;

	UPROPERTY()
	int32 enhance;
};

UCLASS()
class G_API UInventorySaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FSoftObjectPath, FIntPair> saved_item_list;

	UPROPERTY()
	int32 saved_current_weigth;

	UPROPERTY()
	int32 saved_max_weight;

	UPROPERTY()
	int32 saved_gold;
	
public:
	void Save(FString slot_name, int32 user_idx, TArray<UItemBase*> item_list, int32& current_weight, int32& max_weight, int32& gold);
	TMap<FSoftObjectPath, FIntPair>& GetItemList();
};
