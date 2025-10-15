// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SamiraSkill/ShadowStep/SkillShadowStep.h"
#include <G/PlayerCharacter.h>

#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillShadowStep::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillShadowStep::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillShadowStep::UseSkill()
{
	UseShadowStepSkill();
}

void USkillShadowStep::UseShadowStepSkill()
{
	FVector attackPos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();
	_myCharacter->SetActorLocation(attackPos);
	TargetBuff(_myCharacter);
}

void USkillShadowStep::SkillEnd()
{
}
