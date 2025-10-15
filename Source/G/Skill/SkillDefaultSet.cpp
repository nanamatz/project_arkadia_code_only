// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDefaultSet.h"

#include "SkillInfoManager.h"
#include "G/G.h"
#include "G/GCharacter.h"
#include "G/BT/Task/BTTask_FindTarget.h"
#include "SkillSystemComponent/SkillSystemComponent.h"

bool USkillDefaultSet::CheckSelectPossibility()
{
	return Super::CheckSelectPossibility();
}

bool USkillDefaultSet::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillDefaultSet::SkillStart()
{
	Super::SkillStart();

	LOG(Log, "SkillStart %d Action", GetSkillInfoManager()->GetActionList().Num());
	for (auto& action : GetSkillInfoManager()->GetActionList())
	{
		action->SkillAction(_myCharacter, this);
	}
}

void USkillDefaultSet::UseSkill()
{
	Super::UseSkill();
	LOG(Log, "UseSkill");

	if (GetSkillInfoManager()->GetSkillRangeType() == ESkillRangeType::CHARACTERCIRCLE)
	{
		auto target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	
		TargetBuff(target);
		TargetDebuff(target);
		TargetAttack(target);
	}
	else
	{
		TArray<AGCharacter*> characterList = GetSkillPosOverlapCharacters();
		for (auto character : characterList)
		{
			TargetBuff(character);
			TargetDebuff(character);
			TargetAttack(character);
		}
		LOG(Log, "Not Find Target");
	}
}

void USkillDefaultSet::SkillEnd()
{
	Super::SkillEnd();

	LOG(Log, "SkillEnd");
}
