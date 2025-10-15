// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "G/Util/Define.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Manager/CombatManager.h"
#include "G/Manager/FXManager.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	bDeath = false;
}

// 매 프레임마다 수행(큰 오버헤드는 아님)
void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();
	if (IsValid(pawn))
	{
		speed = pawn->GetVelocity().Size(); // speed??pawn??velocity size�?받아 ??
	}
}

void UCharacterAnimInstance::PlayAnimMontage(class UAnimMontage* _playMontage)
{
	if (_playMontage && !Montage_IsPlaying(_playMontage))
	{
		Montage_Play(_playMontage);
	}
}

void UCharacterAnimInstance::PlayRespawnMontage()
{
	//respawn 애니메이션 실행
	bDeath = false;
}


void UCharacterAnimInstance::PlayPrimaryAttackMontage()
{
	// montage가 실행 중이 아니라면 실행
	if (!Montage_IsPlaying(primary_attack_montage))
	{
		Montage_Play(primary_attack_montage);
	}
}

void UCharacterAnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(death_montage))
	{
		Montage_Play(death_montage);
	}
	bDeath = true;
}

void UCharacterAnimInstance::PlayHitMontage()
{
	if (!Montage_IsPlaying(hit_montage))
	{
		Montage_Play(hit_montage);
	}
}

void UCharacterAnimInstance::PlayBuffMontage(enum class EBUFFTYPE type)
{
	APawn* myPawn = TryGetPawnOwner();
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetFXManager()->PlayVFX<EBUFFTYPE>(type, myPawn->GetActorLocation());
	
}

void UCharacterAnimInstance::PlayDebuffMontage(enum class EDEBUFFTYPE type)
{
	APawn* myPawn = TryGetPawnOwner();
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetFXManager()->PlayVFX<EDEBUFFTYPE>(type, myPawn->GetActorLocation());

	switch (type)
	{
	case EDEBUFFTYPE::STUN:
		GameInstance->GetFXManager()->AttachHeadVFX(myPawn, EHeadVFX::Stun);
		break;
		
	default:
		break;
	}
}
