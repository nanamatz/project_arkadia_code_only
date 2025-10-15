// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillHolySanctuary.generated.h"

/**
 * 
 */
USTRUCT()
struct FHolySanctuaryInfo
{
	GENERATED_USTRUCT_BODY()

	bool bBlessed;
	bool bDurationUp;
};

UCLASS()
class G_API USkillHolySanctuary : public USkillBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FHolySanctuaryInfo holySanctuaryInfo;

	UPROPERTY()
	TSubclassOf<class AHolyArea> holyArea;

public:
	USkillHolySanctuary();

	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;
	virtual void UseSkill() override;
	void SpawnHolyArea();
	virtual void SkillEnd() override;
};
