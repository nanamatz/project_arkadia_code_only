// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/CardenioSkill/HolySanctuary/SkillHolySanctuary.h"
#include <G/PlayerCharacter.h>
#include <G/Skill/CardenioSkill/HolySanctuary/HolyArea.h>
#include <G/Skill/SkillInfoManager.h>

#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

USkillHolySanctuary::USkillHolySanctuary()
{
	static ConstructorHelpers::FClassFinder<AHolyArea> HolyAreaClassFinder(
		TEXT("Blueprint'/Game/Character/Cardenio/Skill_HolySanctuary/BP_HolyArea.BP_HolyArea_C'"));

	if (HolyAreaClassFinder.Succeeded())
	{
		holyArea = HolyAreaClassFinder.Class;
	}
}

bool USkillHolySanctuary::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillHolySanctuary::SkillStart()
{
	Super::SkillStart();
}

void USkillHolySanctuary::UseSkill()
{
	Super::UseSkill();
	SpawnHolyArea();
}

void USkillHolySanctuary::SpawnHolyArea()
{
	FVector attackPos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();
	//T* Projectile = World->SpawnActor<T>(_projectile, SpawnLocation, Rotation, SpawnParams);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = _myCharacter;
	FVector SpawnLocation = attackPos;
	FRotator Rotation = _myCharacter->GetActorForwardVector().Rotation();
	AHolyArea* spawnHolyArea = _myCharacter->GetWorld()->SpawnActor<AHolyArea>(
		holyArea, SpawnLocation, Rotation, SpawnParams);
	if (spawnHolyArea)
	{
		spawnHolyArea->Init(this, _myCharacter);
	}
	else
	{
	}
}

void USkillHolySanctuary::SkillEnd()
{
	Super::SkillEnd();
	auto targetList = GetSkillPosOverlapCharacters();
	for(auto target : targetList)
	{
		TargetBuff(target);
	}
}
