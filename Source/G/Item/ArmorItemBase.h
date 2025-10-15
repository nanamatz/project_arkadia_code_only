// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipItemBase.h"
#include "ArmorItemBase.generated.h"

/**
 * 
 */

UCLASS()
class G_API UArmorItemBase : public UEquipItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 BaseArmor;

	UPROPERTY(EditAnywhere)
	int32 IncreaseArmor;

	virtual bool Equip(enum ECharacterName n) override;
	virtual void Unequip() override;
	
	int GetArmor() { return BaseArmor + (IncreaseArmor * EnhenceValue); }

	virtual FString GetItemType() override
	{
		return FString("Equipment");
	}
};
