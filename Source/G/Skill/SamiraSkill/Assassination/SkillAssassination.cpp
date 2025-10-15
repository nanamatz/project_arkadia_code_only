// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SamiraSkill/Assassination/SkillAssassination.h"
#include <G/PlayerCharacter.h>

#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillAssassination::CheckSkillPossibility()
{
	AGCharacter* target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();

	bool result = true;
	result = CheckTargetInAttackRange(target);

	// 사거리 밖에 있으면 스킬 사용 X
	if (!result)
	{
		return result;
	}

	return Super::CheckSkillPossibility();
}

void USkillAssassination::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillAssassination::UseSkill()
{
	Super::UseSkill();
	UWorld* World = _myCharacter->GetWorld();
	auto target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();

	TargetAttack(target);
	TargetDebuff(target);

	for(auto action : skillInfoManager->GetActionList())
	{
		action->SkillAction(GetMyCharacter(), this);
	}
}

void USkillAssassination::SkillEnd()
{
	Super::SkillEnd();
}
