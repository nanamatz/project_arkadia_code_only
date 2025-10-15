// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCinda/ArrowRain/SkillArrowRain.h"

#include "G/G.h"
#include "Particles/ParticleSystemComponent.h"
#include "G/PlayerCharacter.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "Kismet/GameplayStatics.h"

USkillArrowRain::USkillArrowRain()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ArrowRainParticleSystemFind(
		TEXT(
			"ParticleSystem'/Game/Character/RuthCinda/FX/Particles/Sparrow/Skins/Rogue/P_RainofArrows.P_RainofArrows'"));

	if (ArrowRainParticleSystemFind.Succeeded())
	{
		_arrowRainParticleSystem = ArrowRainParticleSystemFind.Object;
	}
}

bool USkillArrowRain::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillArrowRain::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillArrowRain::UseSkill()
{
	Super::UseSkill();
	ArrowRain();
}

void USkillArrowRain::ArrowRain()
{
	FVector SpawnPos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();

	// 생성된 파티클 저장
	UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
		_myCharacter->GetWorld(), _arrowRainParticleSystem, SpawnPos, _myCharacter->GetActorRotation());


	FTimerHandle TimerHandle;
	_myCharacter->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		TArray<AGCharacter*> Characters = GetSkillPosOverlapCharacters();
		for (AGCharacter* character : Characters)
		{
			TargetAttack(character);
			TargetDebuff(character);
		}

		ESkillTargetType originType = GetSkillInfoManager()->GetTargetType(); 
		GetSkillInfoManager()->SetTargetType(ESkillTargetType::PLAYER);
		Characters = GetSkillPosOverlapCharacters();
		for (AGCharacter* character : Characters)
		{
			TargetBuff(character);
		}
		GetSkillInfoManager()->SetTargetType(originType);
		
	}, 1.0f, false);
}


void USkillArrowRain::SkillEnd()
{
	Super::SkillEnd();
}
