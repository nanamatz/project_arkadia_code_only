#pragma once
#include "CoreMinimal.h"
#include "ActionBase.h"
#include "KilltoRecoverAction.generated.h"

UCLASS()
class UKilltoRecoverAction : public UActionBase
{
	GENERATED_BODY()
public:
	
	virtual void SkillAction(class AGCharacter* myCharacter, class USkillBase* skill) override;
};
