#pragma once
#include "CoreMinimal.h"
#include "ActionBase.h"
#include "JumpAction.generated.h"

UCLASS()
class UJumpAction : public UActionBase
{
	GENERATED_BODY()

public:
	virtual void SkillAction(class AGCharacter* myCharacter, class USkillBase* skill) override;

	void JumpAction();
};
