// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCinda/Voley/SkillVoley.h"

#include "G/PlayerCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/RuthCindaProjectile/RuthCindaVoleyProjectile.h"


USkillVoley::USkillVoley()
{
	static ConstructorHelpers::FClassFinder<ARuthCindaVoleyProjectile> ProjectileClassFinder(
		TEXT(
			"Blueprint'/Game/Character/RuthCinda/Projectile/BP_RuthCindaVoleyProjectile.BP_RuthCindaVoleyProjectile_C'"));
	if (ProjectileClassFinder.Succeeded())
	{
		projectileClass = ProjectileClassFinder.Class;
	}
}

bool USkillVoley::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillVoley::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillVoley::UseSkill()
{
	float RadiusList1[5] = {-30.f, -15.f, 0.f, 15.f, 30.f};
	float RadiusList2[6] = {-30.f, -20.f, -10.f, 10.f, 20.f, 30.f};
	FRotator originRotation = _myCharacter->GetActorForwardVector().Rotation();
	bool bAddProjectile = (GetSkillInfoManager()->GetAttackList().Num() != 1);
	int projectileNum = 5 + bAddProjectile; 
	for(int i = 0; i < projectileNum; i++)
	{
		if(bAddProjectile)
		{
			_myCharacter->SetActorRotation(originRotation + FRotator(0, RadiusList2[i], 0));	
		}
		else
		{
			_myCharacter->SetActorRotation(originRotation + FRotator(0, RadiusList1[i], 0));
		}
		Super::UseSkill();
	}
	_myCharacter->SetActorRotation(originRotation);
}

void USkillVoley::SkillEnd()
{
	Super::SkillEnd();
}

void USkillVoley::SpawnTargetProjectile(int _attackIndex)
{
	Super::SpawnTargetProjectile(_attackIndex);
}

void USkillVoley::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex)
{
	Super::ProjectileBeginOverlap(OtherActor, attackIndex);
}
