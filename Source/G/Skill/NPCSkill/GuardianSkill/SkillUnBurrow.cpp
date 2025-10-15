// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUnBurrow.h"

#include "G/GCharacter.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"


bool USkillUnBurrow::CheckSelectPossibility()
{
	int useCost = skillInfoManager->GetCost();
	int coolDown = skillInfoManager->GetCurCoolDown();
	auto animInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
	if (_myCharacter->GetCurrentCost() >= useCost && coolDown <= 0 && (animInstance->GetAnimState() ==
		EAnimationState::Burrow))
	{
		return true;
	}
	return false;
}

bool USkillUnBurrow::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillUnBurrow::SkillStart()
{
	Super::SkillStart();
	auto animInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
	animInstance->SetAnimState(EAnimationState::Idle);
	_myCharacter->SetAffectNavigation(true);
	_myCharacter->SetActorEnableCollision(true);
}

void USkillUnBurrow::UseSkill()
{
	Super::UseSkill();
	TargetBuff(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
}

void USkillUnBurrow::SkillEnd()
{
	Super::SkillEnd();
}
