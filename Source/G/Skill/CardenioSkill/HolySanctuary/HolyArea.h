// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillSpawnObjectBase.h"
#include <G/Skill/CardenioSkill/HolySanctuary/SkillHolySanctuary.h>
#include "HolyArea.generated.h"

UCLASS()
class G_API AHolyArea : public ASkillSpawnObjectBase
{
	GENERATED_BODY()

public:
	virtual void Init(USkillBase* _mySkill, AGCharacter* _myCharacter) override;
};
