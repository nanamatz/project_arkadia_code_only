// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillAdvent.generated.h"

/**
 * 
 */
USTRUCT()
struct FSkillAdventInfo
{
	GENERATED_USTRUCT_BODY()
	bool bIFFAdvent; // 아군 피해 X
	bool bBuffAdvent; // 방어도 획득
	bool bMultiHitAdvent; //	다단히트
};

UCLASS()
class G_API USkillAdvent : public USkillBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FSkillAdventInfo skillAdventInfo;

public:
	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;

	virtual void SkillEnd() override;

	void SkillAttack();
};
