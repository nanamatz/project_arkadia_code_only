// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSandBlasting.h"

#include "G/GCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillSandBlasting::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillSandBlasting::SkillStart()
{
	Super::SkillStart();
}

void USkillSandBlasting::UseSkill()
{
	Super::UseSkill();
	auto target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	TargetDebuff(target);
	TargetAttack(target);
}

void USkillSandBlasting::SkillEnd()
{
	Super::SkillEnd();
}
