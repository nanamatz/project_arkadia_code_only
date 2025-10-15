// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillShadowStep.generated.h"

/**
 * 
 */

USTRUCT()
struct FShadowStepInfo
{
	GENERATED_USTRUCT_BODY()
	bool bBuff;
};

UCLASS()
class G_API USkillShadowStep : public USkillBase
{
	GENERATED_BODY()

protected:
	FShadowStepInfo shadowStepInfo;

public:
	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;
	void UseShadowStepSkill();
	virtual void SkillEnd() override;
};
