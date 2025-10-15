// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/CardenioSkill/Preparation/SkillPreparation.h"
#include <G/GCharacter.h>
#include <G/PlayerCharacter.h>

#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"


bool USkillPreparation::CheckSkillPossibility()
{
	if (_myCharacter)
	{
		AGCharacter* target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();

		bool result = true;
		if (target)
		{
			result = CheckTargetInAttackRange(target);
		}
		// 사거리 밖에 있으면 스킬 사용 X
		if (!result)
		{
			return result;
		}
	}
	else
	{
	}
	return Super::CheckSkillPossibility();
}

void USkillPreparation::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillPreparation::UseSkill()
{
	Super::UseSkill();
	if (_myCharacter)
	{
		AGCharacter* target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();
		// 버프 처리
		if (target)
		{
			TargetBuff(target);
		}
	}
}

void USkillPreparation::SkillEnd()
{
	Super::SkillEnd();
}
