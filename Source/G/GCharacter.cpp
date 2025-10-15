// Copyright Epic Games, Inc. All Rights Reserved.

#include "GCharacter.h"
#include <G/CharacterInfoComponent.h>
#include "G/Player/CharacterInfoStruct.h"
#include "CharacterAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "G/Anim/CharacterAnimInstance.h"

#include "G/GGameInstance.h"
#include "G/UI/InGameUI/InGameUI.h"
#include <Kismet/GameplayStatics.h>

#include "G.h"
#include "NavigationInvokerComponent.h"
#include "Manager/CombatManager.h"
#include "Manager/FXManager.h"
#include "Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "UI/TurnQueueUI/TurnQueueWidget.h"

AGCharacter::AGCharacter()
{
	SkillSystemComponent = CreateDefaultSubobject<USkillSystemComponent>(TEXT("SkillSystemComponent"));

	CharacterInfoComponent = CreateDefaultSubobject<UCharacterInfoComponent>(TEXT("CharacterInfoComponent"));
	
	// NavCollision Setting
	NavCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NavCollision"));
	NavCollision->SetupAttachment(GetRootComponent());

	NavCollision->SetCanEverAffectNavigation(false);
	NavCollision->SetCapsuleHalfHeight(200.0f);
	NavCollision->bDynamicObstacle = true;
}

void AGCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AGCharacter::InitCharacter()
{
	CharacterInfoComponent = NewObject<UCharacterInfoComponent>(this);
}

void AGCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	// GetWorld가 유효한지 확인 후 타이머 해제
	if (GetWorld())
	{
		// 타이머가 활성 상태인지 확인하고 해제
		if (GetWorldTimerManager().IsTimerActive(_rotateHandle))
		{
			GetWorldTimerManager().ClearTimer(_rotateHandle);
		}
	}
}
FCharacterInfo& AGCharacter::GetCharacterInfo() { return CharacterInfoComponent->GetCharacterInfo(); }
int32 AGCharacter::GetCurrentHP() { return CharacterInfoComponent->GetCurrentHP(); }
int32 AGCharacter::GetMaxHP() { return CharacterInfoComponent->GetMaxHP(); }
int32& AGCharacter::GetCurrentHPRef() { return CharacterInfoComponent->GetCurrentHPRef(); }
int32& AGCharacter::GetMaxHPRef() { return CharacterInfoComponent->GetMaxHPRef(); }
int32 AGCharacter::GetCurrentCost() { return CharacterInfoComponent->GetCurrentCost(); }
int32 AGCharacter::GetMaxCost() { return CharacterInfoComponent->GetMaxCost(); }
int32 AGCharacter::GetCurrentShield() { return CharacterInfoComponent->GetCurrentShield(); }
int32 AGCharacter::GetMinDamage() { return CharacterInfoComponent->GetMinDamage(); }
int32 AGCharacter::GetMaxDamage() { return CharacterInfoComponent->GetMaxDamage(); }
bool AGCharacter::GetBDeath() { return CharacterInfoComponent->GetBDeath(); }
FText AGCharacter::GetCharacterName() { return CharacterInfoComponent->GetCharacterName(); }
UTexture2D& AGCharacter::GetPortrait() { return CharacterInfoComponent->GetPortrait(); }

ECharacterName AGCharacter::GetECharacterName()
{
	if(!CharacterInfoComponent)
	{
		LOG(Warning, "%s Not Init CharacterInfoComponent", *GetName());
	}
	return CharacterInfoComponent->GetECharacterName();
}

void AGCharacter::FillCost() { CharacterInfoComponent->AddCurrentCost(CharacterInfoComponent->GetMaxCost()); }

int32 AGCharacter::TakeAttackDamage(int32 _attackDamage, bool bPiercingDamage)
{
	int32 damage = 0;
	if (!GetBDeath() && !CharacterInfoComponent->GetBInvulnerability())
	{
		int32 shield = GetCurrentShield();

		for (auto& debuff : CharacterInfoComponent->GetDeBuffListRef())
		{
			if (debuff.type == EDEBUFFTYPE::WEAK)
			{
				damage += debuff.buffAmount;
				break;
			}
		}

		// 데미지 계산식
		if (bPiercingDamage || CheckDebuff(EDEBUFFTYPE::APPLYPIERCEDAMAGE))
		{
			damage = _attackDamage;
		}
		else
		{
			damage = (_attackDamage - shield);
			// 쉴드 먼저 감소
			int32 preShield = CharacterInfoComponent->GetCurrentShield();
			CharacterInfoComponent->AddCurrentShield(-_attackDamage);
			int32 curShield = CharacterInfoComponent->GetCurrentShield();
		}

		if (damage < 0)
		{
			damage = 0.0f;
		}
		else
		{
			// 체력 감소
			CharacterInfoComponent->AddCurrentHP(-damage);
			GameInstance->GetInGameUI()->GetTurnQueueUI()->UpdateHP(this);
		}

		if (GetCurrentHP() <= 0.0f)
		{
			Death();
		}
		else
		{
			auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance)
			{
				AnimInstance->PlayHitMontage();
			}
		}
	}else if(!GetBDeath())
	{
		GameInstance->GetCombatManager()->OnDamageText(GetActorLocation(), 0, EDamageType::Default);
	}
	return damage;
}

void AGCharacter::Respawn()
{
	if (!GetCharacterInfoManager()->GetBDeath())
	{
		return;
	}

	LOG(Log, "%s Respawn", *GetCharacterName().ToString());
	CharacterInfoComponent->SetBDeath(false);

	auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetAnimState(EAnimationState::Idle);
		AnimInstance->PlayRespawnMontage();
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AGCharacter::Death()
{
	if (GetCharacterInfoManager()->GetBDeath())
	{
		return;
	}

	GetController()->StopMovement();
	CharacterInfoComponent->SetBDeath(true);

	// 애니메이션 실행
	auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayDeathMontage();
	}

	SetAffectNavigation(false);
	GameInstance->GetCombatManager()->DeleteCharacterTurnList(this);
	GetWorldTimerManager().ClearTimer(_rotateHandle);
}

int AGCharacter::AddCurrentCost(int _cost)
{
	CharacterInfoComponent->AddCurrentCost(_cost);
	return CharacterInfoComponent->GetCurrentCost();
}

int AGCharacter::AddCurrentHP(float _hp)
{
	CharacterInfoComponent->AddCurrentHP(_hp);
	if (GetECharacterName() != ECharacterName::NonPlayerCharacter &&
		GetECharacterName() != ECharacterName::None)
	{
		GameInstance->GetInGameUI()->UpdateHP(GetECharacterName());
	}

	LOG(Log, "Add Current HP %f", _hp);

	return CharacterInfoComponent->GetCurrentHP();
}

void AGCharacter::SetMoveCost(float _num)
{
	CharacterInfoComponent->SetMoveCost(_num);
}

float AGCharacter::GetMoveCost()
{
	return CharacterInfoComponent->GetMoveCost();
}

void AGCharacter::AddMoveCost(int value)
{
	CharacterInfoComponent->AddMoveCost(
		static_cast<float>(value) * CharacterInfoComponent->GetMovePerCost() * (1 + CharacterInfoComponent->
			GetAbilityValue(EAbility::Swift)/10.0f) * 100.0f
	);
}

void AGCharacter::UseMoveCost(float value)
{
	CharacterInfoComponent->AddMoveCost(-value);
}

void AGCharacter::ApplyBuff(EBUFFTYPE _buffNum, float _amount, int32 _turnCount)
{
	auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayBuffMontage(_buffNum);
	}

	LOG(Log, "ApplyBuff: %s", *UEnum::GetValueAsString(_buffNum));
	// 이미 있는 버프면 카운트만 증가
	for (auto buff : CharacterInfoComponent->GetBuffListRef())
	{
		UE_LOG(LogTemp, Warning, TEXT("New Buff : %s , BuffList : %s | %d"), *UEnum::GetValueAsString(_buffNum),
		       *UEnum::GetValueAsString(buff.type), buff.turnCount);
		if (buff.type == _buffNum)
		{
			buff.turnCount += _turnCount;
			return;
		}
	}

	FBUFF newBuff;
	newBuff.type = _buffNum;
	newBuff.turnCount = _turnCount;
	newBuff.buffAmount = _amount;

	switch (_buffNum)
	{
	case EBUFFTYPE::SHIELDINCREASE:
		CharacterInfoComponent->AddCurrentShield(_amount);
		GameInstance->GetInGameUI()->GetTurnQueueUI()->UpdateHP(this);
		return;
	case EBUFFTYPE::HEALING:
		if (CheckDebuff(EDEBUFFTYPE::HEALINGDECREASE))
		{
			_amount /= 2;
		}
		AddCurrentHP(_amount);
		return;
	case EBUFFTYPE::COSTHEALING:
		CharacterInfoComponent->AddCurrentCost(_amount);
		GameInstance->GetInGameUI()->SetCost(this);
		return;
	case EBUFFTYPE::DEBUFFCLEANS:
		CharacterInfoComponent->GetDeBuffListRef().Empty();
		return;

	case EBUFFTYPE::MOVECOSTINCREASE:
		CharacterInfoComponent->AddMovePerCost(_amount);
		break;
	case EBUFFTYPE::COSTINCREASE:
		CharacterInfoComponent->AddMaxCost(_amount);
		AddCurrentCost(_amount);
		GameInstance->GetInGameUI()->IncreaseMaxCost(this, _amount);
		break;
	case EBUFFTYPE::INSIGHTINCREASE:
		CharacterInfoComponent->AddCurrentInsight(_amount);
		break;
	case EBUFFTYPE::LUCKYINCREASE:
		CharacterInfoComponent->AddCurrentLuck(_amount);
		break;
	case EBUFFTYPE::STRENGTHINCREASE:
		CharacterInfoComponent->AddCurrentStrength(_amount);
		break;
	case EBUFFTYPE::INVULNERABILITY:
		CharacterInfoComponent->SetBInvulnerability(true);
		break;
	default:
		break;
	}

	CharacterInfoComponent->AddBuffList(newBuff);

	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetInGameUI()->GetTurnQueueUI()->AddBuff(this, &newBuff);
}

bool AGCharacter::CheckBuff(EBUFFTYPE type)
{
	for (auto buff : CharacterInfoComponent->GetBuffListRef())
	{
		if (buff.type == type)
		{
			return true;
		}
	}
	return false;
}

void AGCharacter::ApplyDeBuff(EDEBUFFTYPE _deBuffNum, float _amount, int32 _turnCount)
{
	auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayDebuffMontage(_deBuffNum);
	}


	for (auto debuff : CharacterInfoComponent->GetDeBuffListRef())
	{
		if (debuff.type == _deBuffNum)
		{
			debuff.turnCount += _turnCount;
			return;
		}
	}

	FDEBUFF newDeBuff;
	newDeBuff.type = _deBuffNum;
	newDeBuff.turnCount = _turnCount;
	newDeBuff.buffAmount = _amount;

	CharacterInfoComponent->AddDeBuffList(newDeBuff);
	
	switch (_deBuffNum)
	{
	case EDEBUFFTYPE::WEAK:
		CharacterInfoComponent->AddCurrentShield(-_amount);
		GameInstance->GetInGameUI()->GetTurnQueueUI()->UpdateHP(this);
		break;
	case EDEBUFFTYPE::DAMAGEDECREASE:
		break;
	case EDEBUFFTYPE::HEALINGDECREASE:
		break;
	case EDEBUFFTYPE::STUN:
		break;
	case EDEBUFFTYPE::APPLYPIERCEDAMAGE:
		break;
	default:
		break;
	}

	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetInGameUI()->GetTurnQueueUI()->AddDebuff(this, &newDeBuff);
}

bool AGCharacter::CheckDebuff(EDEBUFFTYPE type)
{
	for (auto debuff : CharacterInfoComponent->GetDeBuffListRef())
	{
		if (debuff.type == type)
		{
			return true;
		}
	}
	return false;
}

void AGCharacter::CountBuffAndDeBuff()
{
	for (auto buff : CharacterInfoComponent->GetBuffListRef())
	{
		UE_LOG(LogTemp, Warning, TEXT("Count Buff : %s | %d"), *UEnum::GetValueAsString(buff.type), buff.turnCount);
		if (buff.turnCount == 1)
		{
			int amount = buff.buffAmount;
			amount *= -1;
			switch (buff.type)
			{
			case EBUFFTYPE::MOVECOSTINCREASE:
				CharacterInfoComponent->AddMovePerCost(amount);
				break;
			case EBUFFTYPE::COSTINCREASE:
				AddCurrentCost(amount);
				CharacterInfoComponent->AddMaxCost(amount);
				GameInstance->GetInGameUI()->DecreaseMaxCost(this, amount);
				break;
			case EBUFFTYPE::INSIGHTINCREASE:
				CharacterInfoComponent->AddCurrentInsight(amount);
				break;
			case EBUFFTYPE::LUCKYINCREASE:
				CharacterInfoComponent->AddCurrentLuck(amount);
				break;
			case EBUFFTYPE::STRENGTHINCREASE:
				CharacterInfoComponent->AddCurrentStrength(amount);
				break;
			case EBUFFTYPE::INVULNERABILITY:
				CharacterInfoComponent->SetBInvulnerability(false);
			default:
				break;
			}
		}
	}

	for (auto debuff : CharacterInfoComponent->GetDeBuffListRef())
	{
		UE_LOG(LogTemp, Warning, TEXT("Count DeBuff : %s | %d"), *UEnum::GetValueAsString(debuff.type),
		       debuff.turnCount);
		if (debuff.turnCount == 1)
		{
			int amount = debuff.buffAmount;
			amount += -1;
			switch (debuff.type)
			{
			case EDEBUFFTYPE::WEAK:
				break;
			case EDEBUFFTYPE::DAMAGEDECREASE:
				break;
			case EDEBUFFTYPE::HEALINGDECREASE:
				break;
			case EDEBUFFTYPE::STUN:
				GameInstance->GetFXManager()->DeleteHeadVFX(this, EHeadVFX::Stun);
				break;
			case EDEBUFFTYPE::APPLYPIERCEDAMAGE:
				break;
			default:
				break;
			}
		}
	}


	CharacterInfoComponent->CountBuffAndDeBuff();
}

void AGCharacter::CountSkillCoolDown()
{
	if (!SkillSystemComponent)
	{
		LOG(Log, "SkillSystemComponent NULL");
	}
	else
	{
		SkillSystemComponent->CountSkillCoolDown();
	}
}


void AGCharacter::CharacterCombatTurnStart()
{
	SetAffectNavigation(false);
	CountBuffAndDeBuff();
	CountSkillCoolDown();
}

void AGCharacter::CharacterCombatTurnEnd()
{
	SetAffectNavigation(true);
}

void AGCharacter::SetAffectNavigation(bool b)
{
	auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && AnimInstance->GetAnimState() == EAnimationState::Burrow)
	{
		NavCollision->SetCanEverAffectNavigation(false);
		return;
	}
	NavCollision->SetCanEverAffectNavigation(b);
}

void AGCharacter::RotateTowardTarget(AGCharacter* Target)
{
	if (Target && Target != this && !GetBDeath())
	{
		Target->DeActiveHighlight();

		// 타이머가 이미 활성화되어 있는지 확인하고 취소합니다.
		if (GetWorld()->GetTimerManager().IsTimerActive(_rotateHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(_rotateHandle);
		}

		if(!GetWorld()) return;
		GetWorld()->GetTimerManager().SetTimer(_rotateHandle, [this, Target]
		{
			if(!Target) return;
			FVector LookAtDirection = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

			FRotator TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();

			TargetRotation.Pitch = 0;
			TargetRotation.Roll = 0;

			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(),
			                                        5.0f);
			SetActorRotation(NewRotation);

			// 회전이 완료된 경우 타이머를 취소합니다.
			if (NewRotation.Equals(TargetRotation, 1.0f))
			{
				if(!GetWorld()) return;
				GetWorld()->GetTimerManager().ClearTimer(_rotateHandle);
			}
		}, GetWorld()->GetDeltaSeconds(), true);
	}
}

void AGCharacter::RotateTowardMousePos(FVector MousePos)
{
	FVector Direction = MousePos - GetActorLocation();
	Direction.Normalize();
	FRotator TargetRotation = Direction.ToOrientationRotator();
	TargetRotation.Pitch = 0.0f;
	SetActorRotation(TargetRotation);
}

void AGCharacter::ActiveHighlight()
{
	//UE_LOG(LogTemp, Log, TEXT("Gchar ActiveHighlight (502) - %s"), *(this->GetName()));
	if(GetMesh())
		GetMesh()->SetRenderCustomDepth(true);
}

void AGCharacter::DeActiveHighlight()
{
	//UE_LOG(LogTemp, Log, TEXT("Gchar deActiveHighlight (508) - %s"), *(this->GetName()));
	if(GetMesh())
		GetMesh()->SetRenderCustomDepth(false);
}

void AGCharacter::ActiveHighlightInMoment(float time)
{
	FTimerHandle highlight_timer_handler;

	GetMesh()->SetRenderCustomDepth(true);
	GetWorldTimerManager().SetTimer(highlight_timer_handler, this, &AGCharacter::DeActiveHighlight, time, false);
}
