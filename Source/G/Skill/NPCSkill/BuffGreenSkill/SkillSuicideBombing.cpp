// Fill out your copyright notice in the Description page of Project Settings.
#include "SkillSuicideBombing.h"

#include "G/GCharacter.h"
#include "G/GGameInstance.h"
#include "G/Manager/FXManager.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "Kismet/GameplayStatics.h"

bool USkillSuicideBombing::CheckSkillPossibility()
{
	if(bIsBoob) return true;
	return Super::CheckSkillPossibility();
}

void USkillSuicideBombing::SkillStart()
{
	Super::SkillStart();
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(bIsBoob)
	{
		skillInfoManager->SetTargetType(ESkillTargetType::ALL);
		_myCharacter->GetSkillSystemComponent()->SetSkillPos(_myCharacter->GetActorLocation());
		GetOverlapObjectAttack();
		GameInstance->GetFXManager()->DeleteHeadVFX(_myCharacter, EHeadVFX::Evil);
		_myCharacter->Death();
	}else
	{
		GameInstance->GetFXManager()->AttachHeadVFX(_myCharacter, EHeadVFX::Evil);
	}
}

void USkillSuicideBombing::UseSkill()
{
	Super::UseSkill();
	bIsBoob = true;
}

void USkillSuicideBombing::SkillEnd()
{
	Super::SkillEnd();
}
