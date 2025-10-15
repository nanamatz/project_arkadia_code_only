// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/CardenioSkill/HolySanctuary/HolyArea.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <G/GCharacter.h>
#include <G/Skill/CardenioSkill/HolySanctuary/SkillHolySanctuary.h>
#include <G/PlayerCharacter.h>
#include <G/NonPlayerCharacter.h>
#include <G/GGameMode.h>
#include <G/CombatArea.h>
#include <Kismet/GameplayStatics.h>

#include "G/G.h"
#include "G/Manager/CombatManager.h"


void AHolyArea::Init(USkillBase* _mySkill, AGCharacter* _myCharacter)
{
	Super::Init(_mySkill, _myCharacter);
}
