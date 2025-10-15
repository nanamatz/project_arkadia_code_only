// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillSuicideBombing.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillSuicideBombing : public USkillBase
{
	GENERATED_BODY()
private:
	bool bIsBoob = false;

public:
	virtual bool CheckSkillPossibility() override;
	virtual void SkillStart() override;
	virtual void UseSkill() override;
	virtual void SkillEnd() override;
};
