#include "ActionBase.h"

void UActionBase::SkillAction(class AGCharacter* myCharacter, class USkillBase* mySkill)
{
	_myCharacter = myCharacter;
	_mySkill = mySkill;
}
