// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Item/UsableItemBase.h"

#include "G/Skill/SkillBase.h"

class UGGameInstance;

UUsableItemBase::UUsableItemBase()
{
}

void UUsableItemBase::Use(AGCharacter* Character)
{
}

void UUsableItemBase::Use(APlayerCharacter* Character)
{
	
}

TSubclassOf<USkillBase> UUsableItemBase::GetSkill()
{
	return Skill;
}
