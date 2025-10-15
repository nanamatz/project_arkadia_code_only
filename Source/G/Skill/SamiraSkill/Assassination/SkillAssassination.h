// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillAssassination.generated.h"

/**
 * 
 */

USTRUCT()
struct FAssassinationInfo
{
	GENERATED_USTRUCT_BODY()
	bool bCritical;
	bool bDebuff;
	bool bRecover;
};

UCLASS()
class G_API USkillAssassination : public USkillBase
{
	GENERATED_BODY()

protected:
	FAssassinationInfo assassinationInfo;

public:
	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;
	void UseAssassinationSkill();
	virtual void SkillEnd() override;
};
