// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCindaProjectile/RuthCindaAttackPrimaryProjectile.h"

#include "G/GCharacter.h"
#include "G/Skill/SkillBase.h"
#include "GameFrameWork/ProjectileMovementComponent.h"


ARuthCindaAttackPrimaryProjectile::ARuthCindaAttackPrimaryProjectile()
{
	ProjectileMovement->bIsHomingProjectile = true;
}

void ARuthCindaAttackPrimaryProjectile::SetTargetSpawnProjectile(USkillProjectileBase* skillClass,
	AGCharacter* spawnCharacter, int _attackIndex)
{
	Super::SetTargetSpawnProjectile(skillClass, spawnCharacter, _attackIndex);
}

void ARuthCindaAttackPrimaryProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
