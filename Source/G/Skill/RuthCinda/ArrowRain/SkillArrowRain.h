// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillArrowRain.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillArrowRain : public USkillBase
{
	GENERATED_BODY()

protected:
	// 메테오 이펙트
	UPROPERTY()
	class UParticleSystem* _arrowRainParticleSystem;

public:
	USkillArrowRain();

	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;
	void ArrowRain();

	virtual void SkillEnd() override;
};
