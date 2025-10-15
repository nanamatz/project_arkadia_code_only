// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCinda/RuthCindaAttackPrimary.h"

#include <G/Skill/RuthCindaProjectile/RuthCindaAttackPrimaryProjectile.h>

#include "G/PlayerCharacter.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"


URuthCindaAttackPrimary::URuthCindaAttackPrimary()
{
	static ConstructorHelpers::FClassFinder<ARuthCindaAttackPrimaryProjectile> ProjectileClassFinder(
		TEXT(
			"Blueprint'/Game/Character/RuthCinda/Projectile/BP_RuthCindaAttackPrimaryProjectile.BP_RuthCindaAttackPrimaryProjectile_C'"));
	if (ProjectileClassFinder.Succeeded())
	{
		projectileClass = ProjectileClassFinder.Class;
	}
}

bool URuthCindaAttackPrimary::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void URuthCindaAttackPrimary::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void URuthCindaAttackPrimary::UseSkill()
{
	Super::UseSkill();
}

void URuthCindaAttackPrimary::SkillEnd()
{
	Super::SkillEnd();
}

void URuthCindaAttackPrimary::SpawnTargetProjectile(int _attackIndex)
{
	Super::SpawnTargetProjectile(_attackIndex);
}

void URuthCindaAttackPrimary::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex)
{
	Super::ProjectileBeginOverlap(OtherActor, attackIndex);
}

