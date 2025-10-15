// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/FernandoProjectile/FernandoAttackPrimaryProjectile.h"

#include "G/NonPlayerCharacter.h"
#include "G/Skill/SkillBase.h"
#include "GameFrameWork/ProjectileMovementComponent.h"

AFernandoAttackPrimaryProjectile::AFernandoAttackPrimaryProjectile()
{
	ProjectileMovement->bIsHomingProjectile = true;
}

void AFernandoAttackPrimaryProjectile::SetTargetSpawnProjectile(USkillProjectileBase* skillClass, AGCharacter* spawnCharacter, int _attackIndex)
{
	Super::SetTargetSpawnProjectile(skillClass, spawnCharacter, _attackIndex);
}

void AFernandoAttackPrimaryProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
