// Fill out your copyright notice in the Description page of Project Settings.


#include "ConvertDamageToShieldAction.h"

#include "G/PlayerCharacter.h"
#include "G/Skill/SkillBase.h"

void UConvertDamageToShieldAction::SkillAction(AGCharacter* myCharacter, USkillBase* mySkill)
{
	Super::SkillAction(myCharacter, mySkill);
	ConvertToDamageToShield();
}

void UConvertDamageToShieldAction::ConvertToDamageToShield()
{
	int32 totalDamage = _mySkill->GetTotalDamage();
	_myCharacter->ApplyBuff(EBUFFTYPE::SHIELDINCREASE, totalDamage / 5, 0);
}
