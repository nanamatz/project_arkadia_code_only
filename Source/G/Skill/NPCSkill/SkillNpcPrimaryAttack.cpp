#include "SkillNpcPrimaryAttack.h"

#include "G/G.h"
#include "G/GCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillNpcPrimaryAttack::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillNpcPrimaryAttack::SkillStart()
{
	Super::SkillStart();
	for (auto action : GetSkillInfoManager()->GetActionList())
	{
		action->SkillAction(_myCharacter, this);
	}
}

void USkillNpcPrimaryAttack::UseSkill()
{
	Super::UseSkill();
	LOG(Log, "AttackList Num : %d", GetSkillInfoManager()->GetAttackList().Num());
	for (auto attack : GetSkillInfoManager()->GetAttackList())
	{
		TargetAttack(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
	}
}

void USkillNpcPrimaryAttack::SkillEnd()
{
	Super::SkillEnd();
}
