// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/FernandoSkill/Meteor/SkillMeteor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <G/Skill/SkillInfoManager.h>
#include "Particles/ParticleSystemComponent.h"
#include <G/PlayerCharacter.h>
#include <G/Skill/FernandoSkill/Meteor/MeteorCrater.h>

#include "G/G.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

USkillMeteor::USkillMeteor()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PreMeteorFinder(
		TEXT("/Game/Character/Fernando/Skill_Meteor/P_Gideon_Meteor_Portal_Undertow"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MeteorFinder(
		TEXT("/Game/Character/Fernando/Skill_Meteor/P_Gideon_MeteorBigImpact_Undertow"));

	if (PreMeteorFinder.Succeeded() && MeteorFinder.Succeeded())
	{
		PreMeteorParticle = PreMeteorFinder.Object;
		MeteorParticle = MeteorFinder.Object;
		if (PreMeteorParticle && MeteorParticle)
		{
			// Meteor1ParticleSystem을 사용하여 원하는 작업 수행
		}
		else
		{
		}
	}
	else
	{
	}

	static ConstructorHelpers::FClassFinder<AMeteorCrater> MeteorCraterClassFinder(
		TEXT("Blueprint'/Game/Character/Fernando/Skill_Meteor/BP_MeteorCrater.BP_MeteorCrater_C'"));
	if (MeteorCraterClassFinder.Succeeded())
	{
		meteorCrater = MeteorCraterClassFinder.Class;
	}
}


bool USkillMeteor::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillMeteor::SkillStart()
{
	// 애니메이션 실행
	Super::SkillStart();
}

void USkillMeteor::UseSkill()
{
	UseMeteor();
}

void USkillMeteor::UseMeteor()
{
	FTimerHandle MeteorDelayTimerHandle;
	// 0.34초 뒤에 Projectile 생성
	UWorld* World = _myCharacter->GetWorld();
	PreMeteor();
	World->GetTimerManager().SetTimer(MeteorDelayTimerHandle, [this]() { DropMeteor(); }, 3.28f, false);
}

void USkillMeteor::PreMeteor()
{
	UWorld* World = _myCharacter->GetWorld();

	FVector AttackPos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();
	FRotator Rotation = _myCharacter->GetActorRotation();
	// 생성된 파티클 저장
	UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
		World, PreMeteorParticle, AttackPos, Rotation);
	// 스킬 사이즈 가져오기
	float SkillRange = skillInfoManager->GetMouseRange();
	// 스케일 조절
	SkillRange /= 400.0f;
	ParticleSystemComponent->SetRelativeScale3D(FVector(SkillRange, SkillRange, SkillRange));
}

void USkillMeteor::DropMeteor()
{
	UWorld* World = _myCharacter->GetWorld();
	// 생성된 Projectile은 AttackTarget을 향해 날아감.
	// 파티클 살짝 위에 생성
	FVector EffectLocation = _myCharacter->GetSkillSystemComponent()->GetSkillPos();
	FRotator Rotation = _myCharacter->GetActorRotation();
	EffectLocation.Z += 150.0f;
	// 생성된 파티클 저장
	UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
		World, MeteorParticle, EffectLocation, Rotation);
	// 스킬 사이즈 가져오기
	float SkillRange = skillInfoManager->GetMouseRange();
	// 스케일 조절
	SkillRange /= 400.0f;
	ParticleSystemComponent->SetRelativeScale3D(FVector(SkillRange, SkillRange, SkillRange));
	SkillEnd();
}

void USkillMeteor::SkillEnd()
{
	TArray<AGCharacter*> characters = GetSkillPosOverlapCharacters();

	for(auto attack : GetSkillInfoManager()->GetAttackList())
	{
		LOG(Log, "%d SpawnCrater", attack.Duration);
		if(attack.Duration > 0)
		{
			SpawnCrater(attack);
		}
	}

	for (AGCharacter* character : characters)
	{
		TargetAttack(character);
		TargetDebuff(character);
		TargetBuff(character);
	}
}

void USkillMeteor::SpawnCrater(FSkillCastAttack attack)
{
	LOG(Log, "SpawnCrater");
	FVector attackPos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = _myCharacter;
	FVector SpawnLocation = attackPos;
	FRotator Rotation = _myCharacter->GetActorForwardVector().Rotation();
	AMeteorCrater* spawnCrater = _myCharacter->GetWorld()->SpawnActor<AMeteorCrater>(
		meteorCrater, SpawnLocation, Rotation, SpawnParams);
	if (spawnCrater)
	{
		float attackDamage = attack.MinDamage;
		float size = skillInfoManager->GetMouseRange();
		float duration = attack.Duration;
		spawnCrater->Init(this, _myCharacter);
	}
	else
	{
		LOG(Warning, "Fail Spawn Crater");
	}
}
