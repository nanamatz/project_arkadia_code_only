// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UTexture2D;
class UInventorySlotWidget;

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(Meta = (Bitflags))
enum class EItemType
{
	Consumables = 100,
	Material,
	KeyItems,
	Equipment
};

UCLASS(Blueprintable)
class G_API UItemBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UInventorySlotWidget* ui_slot = nullptr;

	FName ItemID;

	UPROPERTY(EditAnywhere)
	UTexture2D* item_icon;

public:
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EEquipType"))
	EItemType ItemType;

	UPROPERTY(EditAnywhere)
	/* weight of one */
	int32 weight;

	UPROPERTY(EditAnywhere)
	/* value of one */
	int32 value;

	/* total number of having */
	int32 count = 1;

	UPROPERTY(EditAnywhere)
	FText Description;

	virtual FString GetItemType()
	{
		switch (ItemType)
		{
		case EItemType::Consumables:
			return FString("Consumables"); //소모품
		case EItemType::Material:
			return FString("Material"); //재료
		case EItemType::KeyItems:
			return FString("KeyItems"); //중요 아이템
		default:
			return FString("ItemBase");
		}
	}


	void SetSlotUI(UInventorySlotWidget* _ui_slot);
	void UpdateSlotUI();

	UTexture2D* GetIcon() { return item_icon; };
	UTexture2D*& GetIconRef() { return item_icon; };
};
