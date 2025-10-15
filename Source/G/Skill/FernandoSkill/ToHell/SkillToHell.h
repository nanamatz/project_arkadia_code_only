// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillToHell.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillToHell : public USkillBase
{
	GENERATED_BODY()

public:
	USkillToHell();

	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;

	virtual void SkillEnd() override;
};
