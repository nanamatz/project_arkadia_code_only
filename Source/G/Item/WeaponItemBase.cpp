// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemBase.h"

bool UWeaponItemBase::Equip(ECharacterName n)
{
	ability.Value[EAbility::Power] += BasePower;
	
	return Super::Equip(n);
}

void UWeaponItemBase::Unequip()
{
	ability.Value[EAbility::Power] -= BasePower;
	
	Super::Unequip();
}
