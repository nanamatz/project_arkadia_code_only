// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterResurrectionAction.h"

#include "G/G.h"
#include "G/GCharacter.h"
#include "G/GGameInstance.h"
#include "G/Manager/CombatManager.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UCharacterResurrectionAction::SkillAction(AGCharacter* myCharacter, USkillBase* mySkill)
{
	Super::SkillAction(myCharacter, mySkill);
	LOG(Log, "CharacterResurrection");
	CharacterResurrection();
}

void UCharacterResurrectionAction::CharacterResurrection()
{
	auto target = _myCharacter->GetSkillSystemComponent()->GetSkillTarget();

	target->Respawn();
	target->AddCurrentHP(1);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(_myCharacter->GetWorld()));
	GameInstance->GetCombatManager()->AddTurnCharacter(target);
}
