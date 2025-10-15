// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillCutThroat.generated.h"

/**
 * 
 */

USTRUCT()
struct FCutThroatInfo
{
	GENERATED_USTRUCT_BODY()
	bool bCriticalUp;
	bool bMultiHit;
	bool bDebuff;
};

UCLASS()
class G_API USkillCutThroat : public USkillBase
{
	GENERATED_BODY()

protected:
	FCutThroatInfo cutThroatInfo;

public:
	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;
	void UseCutThroatSkill();
	void AttackTarget();
	virtual void SkillEnd() override;
};
