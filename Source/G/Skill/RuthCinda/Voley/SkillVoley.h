// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillProjectileBase.h"
#include "SkillVoley.generated.h"

/**
 * 
 */

UCLASS()
class G_API USkillVoley : public USkillProjectileBase
{
	GENERATED_BODY()
public:
	USkillVoley();

	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;

	virtual void SkillEnd() override;

	virtual void SpawnTargetProjectile(int _attackIndex) override;
	
	virtual void ProjectileBeginOverlap(AActor* OtherActor, int attackIndex) override;
};
