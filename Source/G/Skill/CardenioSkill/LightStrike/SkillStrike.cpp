// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/CardenioSkill/LightStrike/SkillStrike.h"
#include <G/NonPlayerCharacter.h>
#include <G/Skill/SkillInfoManager.h>

#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"


bool USkillStrike::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillStrike::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
	for (auto action : skillInfoManager->GetActionList())
	{
		action->SkillAction(_myCharacter, this);
	}
}

void USkillStrike::UseSkill()
{
	Super::UseSkill();
	float mouseRange = GetSkillInfoManager()->GetMouseRange();
	if (mouseRange > 0)
	{
		TArray<AGCharacter*> targetList = GetSkillPosOverlapCharacters();
		for(auto target : targetList)
		{
			StrikeAttack(target);
		}
		
	}
	else if (_myCharacter->GetSkillSystemComponent()->GetSkillTarget())
	{
		StrikeAttack(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
	}
}

void USkillStrike::SkillEnd()
{
	Super::SkillEnd();
}

void USkillStrike::StrikeAttack(AGCharacter* _target)
{
	TargetAttack(_target);
	TargetDebuff(_target);
}
