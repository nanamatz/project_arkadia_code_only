// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/ProjectileBase.h"
#include "RuthCindaAttackPrimaryProjectile.generated.h"

/**
 * 
 */
UCLASS()
class G_API ARuthCindaAttackPrimaryProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ARuthCindaAttackPrimaryProjectile();

	virtual void SetTargetSpawnProjectile(USkillProjectileBase* skillClass, AGCharacter* spawnCharacter, int _attackIndex) override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
