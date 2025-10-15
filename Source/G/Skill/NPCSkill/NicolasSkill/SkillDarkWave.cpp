#include "SkillDarkWave.h"

#include "G/GCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillDarkWave::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillDarkWave::SkillStart()
{
	Super::SkillStart();
}

void USkillDarkWave::UseSkill()
{
	Super::UseSkill();

	_myCharacter->GetSkillSystemComponent()->SetSkillPos(_myCharacter->GetActorLocation());
	GetSkillInfoManager()->SetTargetType(ESkillTargetType::ALL);
	TArray<AGCharacter*> characterList = GetSkillPosOverlapCharacters();
	for (auto character : characterList)
	{
		TargetBuff(character);
		TargetDebuff(character);
		TargetAttack(character);
	}
}

void USkillDarkWave::SkillEnd()
{
	Super::SkillEnd();
}
