// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Item/PotionBase.h"

#include "G/PlayerCharacter.h"

UPotionBase::UPotionBase()
{
	ItemType = EItemType::Consumables;
}

void UPotionBase::Use(APlayerCharacter* Character)
{
	Super::Use(Character);

	Character->AddCurrentHP(HP_Increase);
}
