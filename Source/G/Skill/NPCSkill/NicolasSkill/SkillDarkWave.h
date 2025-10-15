#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillDarkWave.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillDarkWave : public USkillBase
{
	GENERATED_BODY()

public:
	virtual bool CheckSkillPossibility() override;
	virtual void SkillStart() override;
	virtual void UseSkill() override;
	virtual void SkillEnd() override;
};