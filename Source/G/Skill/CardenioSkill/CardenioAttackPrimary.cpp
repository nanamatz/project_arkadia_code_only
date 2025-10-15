// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/CardenioSkill/CardenioAttackPrimary.h"

#include "G/G.h"
#include "G/PlayerCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool UCardenioAttackPrimary::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void UCardenioAttackPrimary::SkillStart()
{
	LOG(Log, "UCardenioAttackPrimary");
	// 애니메이션 실행
	Super::SkillStart();
}

void UCardenioAttackPrimary::UseSkill()
{
	Super::UseSkill();
	LOG(Log, "CardenioAttack 0 ");
	for (auto attack : GetSkillInfoManager()->GetAttackList())
	{
		LOG(Log, "CardenioAttack");
		TargetAttack(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
	}
}

void UCardenioAttackPrimary::SkillEnd()
{
	Super::SkillEnd();
}
