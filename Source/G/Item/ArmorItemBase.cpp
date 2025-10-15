// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItemBase.h"

bool UArmorItemBase::Equip(ECharacterName n)
{
	ability.Value[EAbility::Power] += BaseArmor;
	
	return Super::Equip(n);
}

void UArmorItemBase::Unequip()
{
	ability.Value[EAbility::Power] -= BaseArmor;
	
	Super::Unequip();
}
