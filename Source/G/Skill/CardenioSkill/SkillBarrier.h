// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillBarrier.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillBarrier : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void UseSkill() override;
};
