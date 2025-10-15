// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillData/SkillAction/ActionBase.h"
#include "ConvertDamageToShieldAction.generated.h"

/**
 * 
 */
UCLASS()
class G_API UConvertDamageToShieldAction : public UActionBase
{
	GENERATED_BODY()

public:
	virtual void SkillAction(AGCharacter* myCharacter, USkillBase* mySkill) override;

	void ConvertToDamageToShield();
};
