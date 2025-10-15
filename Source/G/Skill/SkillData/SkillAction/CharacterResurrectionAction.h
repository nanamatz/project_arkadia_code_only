// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "CharacterResurrectionAction.generated.h"

/**
 * 
 */
UCLASS()
class G_API UCharacterResurrectionAction : public UActionBase
{
	GENERATED_BODY()

	virtual void SkillAction(AGCharacter* myCharacter, USkillBase* mySkill) override;

	void CharacterResurrection();
};
