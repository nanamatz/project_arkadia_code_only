// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillSpawnCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class G_API USkillSpawnCharacter : public USkillBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere)
	TSubclassOf<class AGCharacter> _spawnCharacter;

public:
	virtual bool CheckSkillPossibility() override;
	virtual void SkillStart() override;
	virtual void UseSkill() override;
	virtual void SkillEnd() override;
};
