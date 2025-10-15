// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillMeteor.generated.h"

/**
 * 
 */

UCLASS()
class G_API USkillMeteor : public USkillBase
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TSubclassOf<class AMeteorCrater> meteorCrater;

	UPROPERTY()
	class UParticleSystem* PreMeteorParticle;

	UPROPERTY()
	class UParticleSystem* MeteorParticle;

public:
	USkillMeteor();

	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;
	void UseMeteor();
	void PreMeteor();
	void DropMeteor();

	virtual void SkillEnd() override;

	void SpawnCrater(struct FSkillCastAttack attack);
};
