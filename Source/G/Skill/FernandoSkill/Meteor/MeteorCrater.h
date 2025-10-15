// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillSpawnObjectBase.h"
#include <G/Skill/FernandoSkill/Meteor/SkillMeteor.h>
#include "MeteorCrater.generated.h"

UCLASS()
class G_API AMeteorCrater : public ASkillSpawnObjectBase
{
	GENERATED_BODY()
public:
	virtual void Init(USkillBase* _mySkill, AGCharacter* _myCharacter) override;
};
