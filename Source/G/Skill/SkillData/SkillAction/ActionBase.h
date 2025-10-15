#pragma once

#include "CoreMinimal.h"
#include "ActionBase.generated.h"

UCLASS()
class UActionBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class AGCharacter* _myCharacter;

	UPROPERTY()
	class USkillBase* _mySkill;

public:
	UFUNCTION()
	virtual void SkillAction(class AGCharacter* myCharacter, class USkillBase* mySkill);
};
