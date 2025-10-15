// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/FernandoSkill/ToHell/SkillToHell.h"
#include <G/PlayerCharacter.h>
#include <G/GCharacter.h>
#include <G/Skill/SkillInfoManager.h>

#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

USkillToHell::USkillToHell()
{
}

bool USkillToHell::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillToHell::SkillStart()
{
	Super::SkillStart();
}

void USkillToHell::UseSkill()
{
	Super::UseSkill();
	
	TargetAttack(_myCharacter);
	AGCharacter* target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	if(target)
	{
		TargetBuff(target);
		TargetDebuff(target);
	}
}

void USkillToHell::SkillEnd()
{
}
