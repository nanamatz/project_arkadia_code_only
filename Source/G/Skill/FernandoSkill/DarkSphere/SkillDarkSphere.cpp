// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/FernandoSkill/DarkSphere/SkillDarkSphere.h"
#include <G/PlayerCharacter.h>
#include <G/Skill/SkillInfoManager.h>
#include "G/Skill/ProjectileBase.h"

USkillDarkSphere::USkillDarkSphere()
{
	static ConstructorHelpers::FClassFinder<AProjectileBase> attackProjectilClass(
		TEXT(
			"Blueprint'/Game/Character/Fernando/Projectile/BP_FernandoAttackPrimaryProjectile.BP_FernandoAttackPrimaryProjectile_C'"));
	if (attackProjectilClass.Succeeded())
	{
		projectileClass = attackProjectilClass.Class;
	}
}

bool USkillDarkSphere::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillDarkSphere::SkillStart()
{
	Super::SkillStart();
}

void USkillDarkSphere::UseSkill()
{
	Super::UseSkill();
}

void USkillDarkSphere::SkillEnd()
{
	Super::SkillEnd();
}

void USkillDarkSphere::SpawnTargetProjectile(int _attackIndex)
{
	Super::SpawnTargetProjectile(_attackIndex);
}

void USkillDarkSphere::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex)
{
	if(OtherActor->IsA<AGCharacter>())
	{
		AGCharacter* target = Cast<AGCharacter>(OtherActor);
		TargetAttack(target);
	}
	
	if(GetSkillInfoManager()->GetSkillRangeType() == ESkillRangeType::MOUSECIRCLE)
	{
		auto targetList = GetSkillPosOverlapCharacters();
		for(auto target : targetList)
		{
			TargetAttack(target);
			TargetBuff(target);
			TargetDebuff(target);
		}
	}
}