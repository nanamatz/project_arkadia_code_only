// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/FernandoSkill/DarkPulse/SkillDarkPulse.h"
#include <G/PlayerCharacter.h>
#include <G/Skill/SkillInfoManager.h>
#include "G/Skill/ProjectileBase.h"


USkillDarkPulse::USkillDarkPulse()
{
	static ConstructorHelpers::FClassFinder<AProjectileBase> DarkPulseProjectileClassFinder(
		TEXT(
			"Blueprint'/Game/Character/Fernando/Projectile/BP_FernandoDarkPulseProjectile.BP_FernandoDarkPulseProjectile_C'"));
	if (DarkPulseProjectileClassFinder.Succeeded())
	{
		 projectileClass = DarkPulseProjectileClassFinder.Class;
	}
}

bool USkillDarkPulse::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillDarkPulse::SkillStart()
{
	Super::SkillStart();
}

void USkillDarkPulse::UseSkill()
{
	Super::UseSkill();
}

void USkillDarkPulse::SkillEnd()
{
	Super::SkillEnd();
}

void USkillDarkPulse::SpawnTargetProjectile(int _attackIndex)
{
	Super::SpawnTargetProjectile(_attackIndex);
}

void USkillDarkPulse::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex)
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