// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/ProjectileBase.h"
#include "G/Skill/RuthCinda/Voley/SkillVoley.h"
#include "RuthCindaVoleyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class G_API ARuthCindaVoleyProjectile : public AProjectileBase
{
	GENERATED_BODY()

	ARuthCindaVoleyProjectile();

	virtual void SetTargetSpawnProjectile(USkillProjectileBase* skillClass, AGCharacter* spawnCharacter, int _attackIndex) override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
