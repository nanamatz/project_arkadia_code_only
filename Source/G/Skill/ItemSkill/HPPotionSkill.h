// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "HPPotionSkill.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class G_API UHPPotionSkill : public USkillBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 IncreasingHP = 1;

protected:
	UHPPotionSkill();

public:
	virtual void SkillStart() override;
	virtual void UseSkill() override;
	virtual void SkillEnd() override;
};
