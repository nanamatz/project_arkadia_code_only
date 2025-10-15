// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBurrow.h"

#include "G/G.h"
#include "G/GCharacter.h"
#include "G/Anim/CharacterAnimInstance.h"

bool USkillBurrow::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillBurrow::SkillStart()
{
	Super::SkillStart();
}

void USkillBurrow::UseSkill()
{
	Super::UseSkill();
	LOG(Log, "Burrow");
	auto animInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
	animInstance->SetAnimState(EAnimationState::Burrow);
	_myCharacter->SetAffectNavigation(false);
	_myCharacter->SetActorEnableCollision(false);
}

void USkillBurrow::SkillEnd()
{
	Super::SkillEnd();
}
