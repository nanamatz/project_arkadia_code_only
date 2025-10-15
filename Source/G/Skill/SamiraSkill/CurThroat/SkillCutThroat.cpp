// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SamiraSkill/CurThroat/SkillCutThroat.h"
#include <G/PlayerCharacter.h>

#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillCutThroat::CheckSkillPossibility()
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

void USkillCutThroat::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillCutThroat::UseSkill()
{
	UseCutThroatSkill();
}

void USkillCutThroat::UseCutThroatSkill()
{
	AGCharacter* target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	AttackTarget();

	if (cutThroatInfo.bDebuff == true)
	{
		target->ApplyDeBuff(EDEBUFFTYPE::APPLYPIERCEDAMAGE, 0, 2);
	}

	if (cutThroatInfo.bMultiHit == true)
	{
		FTimerHandle multiTimerHandle;
		UWorld* World = _myCharacter->GetWorld();
		World->GetTimerManager().SetTimer(multiTimerHandle, [this]()
		{
			AttackTarget();
		}, 0.5f, false);
	}
}

void USkillCutThroat::AttackTarget()
{
	AGCharacter* target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	/*float attackDamage = GetAttackDamage() + GetPierceDamage();

	if (cutThroatInfo.bCriticalUp)
	{
		attackDamage *= 1.5f;
	}*/

	TargetAttack(target);
}

void USkillCutThroat::SkillEnd()
{
}
