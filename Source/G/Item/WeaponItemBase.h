// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipItemBase.h"
#include "WeaponItemBase.generated.h"

/**
 * 
 */
UCLASS()
class G_API UWeaponItemBase : public UEquipItemBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	ECharacterName EquipCharName;
	
public:
	UPROPERTY(EditAnywhere)
	int32 BasePower;

	UPROPERTY(EditAnywhere)
	int32 IncreasePower;

	virtual bool Equip(enum ECharacterName n) override;
	virtual void Unequip() override;

	int GetPower() { return BasePower + (IncreasePower * EnhenceValue); }
};
