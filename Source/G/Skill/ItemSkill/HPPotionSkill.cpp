// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPotionSkill.h"

#include "G/G.h"
#include "G/GCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

UHPPotionSkill::UHPPotionSkill()
{
	skillInfoManager = CreateDefaultSubobject<USkillInfoManager>(TEXT("SkillInfoManager"));
}

void UHPPotionSkill::SkillStart()
{
	Super::SkillStart();
}

void UHPPotionSkill::UseSkill()
{
	Super::UseSkill();

	if (_myCharacter)
	{
		TargetBuff(_myCharacter);
	}
	else
	{
		LOG(Log, "_myCharacter is nullptr");
	}
}

void UHPPotionSkill::SkillEnd()
{
	Super::SkillEnd();

	LOG(Log, "");
	_myCharacter->GetSkillSystemComponent()->
	              RemoveSkillSlot(_myCharacter->GetSkillSystemComponent()->selectedSkillSlot);
}
