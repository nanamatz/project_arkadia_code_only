// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCinda/TripleShot/SkillTripleShot.h"

#include "G/Skill/RuthCindaProjectile/RuthCindaProjectile.h"

#include "G/PlayerCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

USkillTripleShot::USkillTripleShot()
{
	static ConstructorHelpers::FClassFinder<ARuthCindaProjectile> ProjectileClassFinder(
		TEXT(
			"Blueprint'/Game/Character/RuthCinda/Projectile/BP_RuthCindaAttackProjectile.BP_RuthCindaAttackProjectile_C'"));
	if (ProjectileClassFinder.Succeeded())
	{
		projectileClass = ProjectileClassFinder.Class;
	}
}

bool USkillTripleShot::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillTripleShot::SkillStart()
{
	Super::SkillStart();
}

void USkillTripleShot::UseSkill()
{
	Super::UseSkill();
}

void USkillTripleShot::SkillEnd()
{
	Super::SkillEnd();
}

void USkillTripleShot::SpawnTargetProjectile(int _attackIndex)
{
	if(GetSkillInfoManager()->GetAttackList().Num() >= 5)
	{
		auto attackList = GetSkillInfoManager()->GetAttackList();
		FSkillCastAttack newAttack;
		for(int i = 0; i < 4; i++)
		{
			newAttack.MinDamage += attackList[i].MinDamage;
			newAttack.MaxDamage += attackList[i].MaxDamage;
		}
		GetSkillInfoManager()->GetAttackList()[4] = newAttack;
		
		Super::SpawnTargetProjectile(4);
	}
	else
	{
		Super::SpawnTargetProjectile();
	
		UWorld* World = _myCharacter->GetWorld();

		for (int i = 0; i < 3; i++)
		{
			FTimerHandle ShotTimerHandle1;
			World->GetTimerManager().SetTimer(ShotTimerHandle1, [this, i]()
			{
				Super::SpawnTargetProjectile(i+1);
			}, i + 1.34f, false);
		}
	}
}

void USkillTripleShot::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex)
{
	Super::ProjectileBeginOverlap(OtherActor, attackIndex);
}
