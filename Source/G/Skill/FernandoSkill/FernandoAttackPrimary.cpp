// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/FernandoSkill/FernandoAttackPrimary.h"
#include "G/Skill/FernandoProjectile/FernandoAttackPrimaryProjectile.h"
#include "Particles/ParticleSystemComponent.h"


UFernandoAttackPrimary::UFernandoAttackPrimary()
{
	static ConstructorHelpers::FClassFinder<AProjectileBase> attackProjectilClass(
		TEXT(
			"Blueprint'/Game/Character/Fernando/Projectile/BP_FernandoAttackPrimaryProjectile.BP_FernandoAttackPrimaryProjectile_C'"));
	if (attackProjectilClass.Succeeded())
	{
		projectileClass = attackProjectilClass.Class;
	}
}

bool UFernandoAttackPrimary::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void UFernandoAttackPrimary::SkillStart()
{
	Super::SkillStart();
}

void UFernandoAttackPrimary::UseSkill()
{
	Super::UseSkill();
}

void UFernandoAttackPrimary::SkillEnd()
{
	Super::SkillEnd();
}

void UFernandoAttackPrimary::SpawnTargetProjectile(int _attackIndex)
{
	Super::SpawnTargetProjectile(_attackIndex);
}

void UFernandoAttackPrimary::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex)
{
	Super::ProjectileBeginOverlap(OtherActor, attackIndex);
}
