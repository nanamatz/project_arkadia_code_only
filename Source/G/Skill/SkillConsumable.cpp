// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillConsumable.h"

#include "G/GCharacter.h"
#include "SkillSystemComponent/SkillSystemComponent.h"

bool USkillConsumable::CheckSelectPossibility()
{
	return Super::CheckSelectPossibility();
}

bool USkillConsumable::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillConsumable::SkillStart()
{
	Super::SkillStart();
}

void USkillConsumable::UseSkill()
{
	Super::UseSkill();
}

void USkillConsumable::SkillEnd()
{
	Super::SkillEnd();
	_myCharacter->GetSkillSystemComponent()->
	              RemoveSkillSlot(_myCharacter->GetSkillSystemComponent()->selectedSkillSlot);
}
