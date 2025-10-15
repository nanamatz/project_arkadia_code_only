// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillNpcApplyBuff.h"

#include "G/GCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillNpcApplyBuff::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillNpcApplyBuff::SkillStart()
{
	Super::SkillStart();
}

void USkillNpcApplyBuff::UseSkill()
{
	Super::UseSkill();

	TargetBuff(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
}

void USkillNpcApplyBuff::SkillEnd()
{
	Super::SkillEnd();
}
