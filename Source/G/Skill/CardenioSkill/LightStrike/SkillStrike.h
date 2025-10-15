// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillStrike.generated.h"

/**
 * 
 */

UCLASS()
class G_API USkillStrike : public USkillBase
{
	GENERATED_BODY()

public:
	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;

	virtual void SkillEnd() override;

	void StrikeAttack(class AGCharacter* _target);
};
