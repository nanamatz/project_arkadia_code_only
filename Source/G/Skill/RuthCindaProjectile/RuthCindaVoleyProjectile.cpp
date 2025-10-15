// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCindaProjectile/RuthCindaVoleyProjectile.h"

ARuthCindaVoleyProjectile::ARuthCindaVoleyProjectile()
{
}

void ARuthCindaVoleyProjectile::SetTargetSpawnProjectile(USkillProjectileBase* skillClass, AGCharacter* spawnCharacter,
	int _attackIndex)
{
	Super::SetTargetSpawnProjectile(skillClass, spawnCharacter, _attackIndex);
}

void ARuthCindaVoleyProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
