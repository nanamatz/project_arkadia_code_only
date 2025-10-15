// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SkillBase.h"
#include <G/Skill/SkillInfoManager.h>
#include <G/Skill/SkillSlot.h>
#include <G/PlayerCharacter.h>
#include "G/Anim/CharacterAnimInstance.h"
#include <G/PlayerCharacter.h>
#include <G/Skill/ProjectileBase.h>
#include <G/NonPlayerCharacter.h>

#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "G/CharacterInfoComponent.h"
#include "G/G.h"
#include "G/Item/WeaponItemBase.h"
#include "G/Manager/CombatManager.h"
#include "Kismet/GameplayStatics.h"
#include "SkillSystemComponent/SkillSystemComponent.h"

USkillBase::USkillBase()
{
}


void USkillBase::InitSkill()
{
	skillInfoManager->SetCurCoolDown(0);
}

float USkillBase::GetSkillCriticalRate()
{
	return skillInfoManager->GetSkillCriticalRate();
}

void USkillBase::SetPlayerCharacter(AGCharacter* _playerCharacter)
{
	_myCharacter = _playerCharacter;
}

void USkillBase::SetSkill(AGCharacter* _playerCharacter, FSkillStruct* _skillInfo)
{
	if (_skillInfo)
	{
		// LOG(Log, "%s %s", *_playerCharacter->GetName(), *_skillInfo->Name.ToString());
		_myCharacter = _playerCharacter;
		skillInfoManager = NewObject<USkillInfoManager>(this);
		FSkillStruct skillInfo = *_skillInfo;
		skillInfoManager->SetSkillInfo(skillInfo);
	}else
	{
		skillInfoManager = NewObject<USkillInfoManager>(this);
	}
}

void USkillBase::SetSkill(AGCharacter* _playerCharacter)
{
	if (!_playerCharacter)
	{
		LOG(Log, "playerCharacter is nullptr");
	}
	_myCharacter = _playerCharacter;
	skillInfoManager = NewObject<USkillInfoManager>(this, BP_SkillInfo);
}

void USkillBase::SetThumnail(UTexture2D* Thumnail)
{
	skillInfoManager->SetThumnail(Thumnail);
}

bool USkillBase::CheckSelectPossibility()
{
	int useCost = skillInfoManager->GetCost();
	int coolDown = skillInfoManager->GetCurCoolDown();
	auto animInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
	if (_myCharacter->GetCurrentCost() >= useCost && coolDown <= 0 && (animInstance->GetAnimState() !=
		EAnimationState::Burrow))
	{
		return true;
	}
	return false;
}

bool USkillBase::CheckSkillPossibility()
{
	bool result = true;

	if (skillInfoManager->GetSkillRangeType() == ESkillRangeType::CHARACTERCIRCLE)
	{
		result = CheckTargetInAttackRange(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
	}
	else if (skillInfoManager->GetSkillRangeType() == ESkillRangeType::CHARACTERPIE)
	{
	}
	else if (skillInfoManager->GetSkillRangeType() == ESkillRangeType::MOUSECIRCLE)
	{
	}

	return result;
}

bool USkillBase::CheckTargetInAttackRange(AGCharacter* Target)
{
	if (Target)
	{
		float dis = FVector::Distance(_myCharacter->GetActorLocation(), Target->GetActorLocation());
		if (dis <= skillInfoManager->GetCharacterRange())
		{
			return true;
		}
	}
	return false;
}


bool USkillBase::CheckForwardSkillPossibility()
{
	FVector StartLocation = _myCharacter->GetActorLocation();
	FVector EndLocation = _myCharacter->GetSkillSystemComponent()->GetSkillTarget()->GetActorLocation();

	// Ray를 시각적으로 표시하기 위한 디버그 라인
	UWorld* World = _myCharacter->GetWorld();
	if (World)
	{
		//DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, true, 1, 0, 10);
	}
	else
	{
	}

	// 충돌 정보를 담을 변수
	FHitResult HitResult;

	// 충돌 행동을 지정하기 위한 설정 객체
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(_myCharacter); // 현재 액터는 충돌 검사에서 제외
	CollisionParams.bTraceComplex = true; // 복잡한 충돌 검사를 사용할지 여부

	// 라인 트레이스 실행
	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ForwardSkillChannel,
	                                            CollisionParams);

	// 충돌한 오브젝트가 NPC인 경우 True 아닌 경우 False
	if (bHit)
	{
		if (HitResult.GetActor()->IsA(ANonPlayerCharacter::StaticClass()))
		{
			return true;
		}
		return false;
	}

	return false;
}


void USkillBase::SkillStart()
{
	if (_myCharacter->GetSkillSystemComponent()->GetSkillTarget() && (GetSkillInfoManager()->GetTargetType() != ESkillTargetType::SELF))
	{
		_myCharacter->RotateTowardTarget(_myCharacter->GetSkillSystemComponent()->GetSkillTarget());
	}
	if (bHasAnim)
	{
		PlaySkillAnimation();
	}

	skillInfoManager->SetCurCoolDown();

	_myCharacter->GetWorld()->GetTimerManager().SetTimer(skillTimerHandle, [this]() { UseSkill(); },
	                                                     skillInfoManager->GetSkillCastTime(), false);
}

void USkillBase::PlaySkillAnimation()
{
	auto AnimInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		UAnimMontage* playAnimMontage = skillInfoManager->GetSkillAnimMontage();
		AnimInstance->PlayAnimMontage(playAnimMontage);
	}
}

void USkillBase::UseSkill()
{
	SkillEnd();
}

void USkillBase::SkillEnd()
{
	auto AnimInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetAnimState(EAnimationState::Idle);
	}
}

struct FSkillStruct& USkillBase::GetSkillInfo()
{
	return skillInfoManager->GetSkillInfo();
}

class UTexture2D* USkillBase::GetSkillThumnail()
{
	return skillInfoManager->GetThumbnail();
}

class USkillBase* USkillBase::GetMySkillClass()
{
	return skillInfoManager->GetSkillClass();
}

float USkillBase::GetCharacterRange()
{
	return skillInfoManager->GetCharacterRange();
}

int USkillBase::GetCost()
{
	return skillInfoManager->GetCost();
}
	

int32 USkillBase::TargetAttack(AGCharacter* target, FSkillCastAttack* _attack)
{
	if (!target && target->GetBDeath())
	{
		return 0;
	}
	LOG(Log, "Attack!");

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(_myCharacter->GetWorld()));
	FVector targetLocation = target->GetActorLocation();
	FVector missLocation = targetLocation;
	FVector attackLocation = targetLocation;
	FVector criticalLocation = targetLocation;
	FVector shieldLocation = targetLocation;
	int32 takeDamage = 0;
	totalDamage = takeDamage;

	TArray<FSkillCastAttack> attackList;
	
	if(_attack != nullptr)
	{
		attackList.Add(*_attack);
		
	}
	else
	{
		attackList = skillInfoManager->GetAttackList();
	}
	
	for (auto attack : attackList)
	{
		if (!CheckHit(target))
		{
			GameInstance->GetCombatManager()->OnDamageText(missLocation, 0, Miss);
			missLocation.Z += 50.0f;
			LOG(Log, "빗나감!");
			continue;
		}
		
		int32 attackDamage = _myCharacter->GetCharacterInfoManager()->GetDamage() + attack.GetRandomDamage();

		if(_myCharacter->GetECharacterName() != ECharacterName::NonPlayerCharacter && _myCharacter->IsA(APlayerCharacter::StaticClass()))
		{
			attackDamage += Cast<APlayerCharacter>(_myCharacter)->GetBoundWeapon()->GetPower();
		}
		
		for (FDEBUFF debuff : _myCharacter->GetCharacterInfoManager()->GetDeBuffListRef())
		{
			if (debuff.type == EDEBUFFTYPE::DAMAGEDECREASE)
			{
				attackDamage -= debuff.buffAmount;
				break;
			}
		}

		const int32 randomNum = FMath::RandRange(0, 100);
		EDamageType damageType = Default;
		if (_myCharacter->GetCharacterInfoManager()->GetCriticalRate() + GetSkillCriticalRate() >= randomNum)
		{
			// 크리티컬 데미지 처리
			attackDamage *= _myCharacter->GetCharacterInfoManager()->GetCriticalDamage();
			LOG(Log, "%d 크리티컬 피해!", attackDamage);
			damageType = Critical;
		}

		takeDamage  = target->TakeAttackDamage(attackDamage, (attack.Type == EAttackType::Pierce));
		if (takeDamage  > 0)
		{
			FVector damageLocation = attackLocation;
			switch (damageType)
			{
			case EDamageType::Critical:
				damageLocation = criticalLocation;
				criticalLocation.Z += 50.0f;
				break;
			case EDamageType::Default:
				attackLocation.Z += 50.0f;
				break;
			}
		
			GameInstance->GetCombatManager()->OnDamageText(damageLocation, takeDamage, damageType);
			LOG(Log, "%s에게 %d의 피해를 입혔습니다.", *target->GetCharacterInfoManager()->GetCharacterName().ToString(), attackDamage);
		}
		if(attackDamage - takeDamage  > 0 )
		{
			GameInstance->GetCombatManager()->OnDamageText(shieldLocation, attackDamage - takeDamage , EDamageType::Shield);
			shieldLocation.Z += 50.0f;
		}

		totalDamage += takeDamage;
	}
	return totalDamage;
}

bool USkillBase::CheckHit(AGCharacter* target)
{
	float avoidRate = target->GetCharacterInfoManager()->GetAvoidRate();
	float hitRate = _myCharacter->GetCharacterInfoManager()->GetHitRate();

	hitRate -= avoidRate;

	if (hitRate >= FMath::RandRange(0, 100))
	{
		return true;
	}

	return false;
}

void USkillBase::TargetBuff(AGCharacter* target)
{
	if (!target)
	{
		return;
	}
	for (auto buff : skillInfoManager->GetBuffList())
	{
		target->ApplyBuff(buff.type, buff.Value, buff.Duration);
	}
}

void USkillBase::TargetDebuff(AGCharacter* target)
{
	if (!target)
	{
		return;
	}
	for (auto Debuff : skillInfoManager->GetDebuffList())
	{
		target->ApplyDeBuff(Debuff.type, Debuff.Value, Debuff.Duration);
	}
}

void USkillBase::GetOverlapObjectAttack(float radius)
{
	TArray<AGCharacter*> Characters = GetSkillPosOverlapCharacters();

	for (AGCharacter* Character : Characters)
	{
		TargetAttack(Character);
	}
}

TArray<AGCharacter*> USkillBase::GetSkillPosOverlapCharacters(float radius)
{
	TArray<AGCharacter*> Characters;
	if (radius == 0)
	{
		radius = skillInfoManager->GetMouseRange();
	}
	FSkillStruct skillInfo;

	// 충돌 행동을 지정하기 위한 설정 객체
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true; // 복잡한 충돌 검사를 사용할지 여부
	
	ESkillTargetType targetType = skillInfoManager->GetTargetType();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	if (targetType == ESkillTargetType::NPC)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
	}
	else if (targetType == ESkillTargetType::PLAYER)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}
	else if(targetType == ESkillTargetType::ALL)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}
	else if(targetType == ESkillTargetType::ALLIgnoreMe)
	{
		CollisionParams.AddIgnoredActor(_myCharacter); // 현재 액터는 충돌 검사에서 제외
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}

	// 결과를 담을 배열
	TArray<FOverlapResult> HitResults;

	FVector spherePos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();
	// 원형 검사 실행
	bool bHit = _myCharacter->GetWorld()->OverlapMultiByObjectType(
		HitResults, spherePos, FQuat::Identity, ObjectTypes,
		FCollisionShape::MakeSphere(radius), CollisionParams);

	//DrawDebugSphere(GetWorld(), spherePos, radius, 16, FColor::Green, false, 5.0f);

	for (auto HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->IsA(AGCharacter::StaticClass()))
		{
			AGCharacter* OverlappedCharacter = Cast<AGCharacter>(HitActor);
			if (!Characters.FindByKey(OverlappedCharacter))
			{
				Characters.Add(OverlappedCharacter);
			}
		}
	}

	return Characters;
}

int USkillBase::GetOverlapObjectAttack()
{
	int ovelapDamage = 0;
	if (_myCharacter)
	{
		TArray<AGCharacter*> Characters = GetSkillPosOverlapCharacters();
		for (AGCharacter* Character : Characters)
		{
			ovelapDamage += TargetAttack(Character);
		}
	}
	return ovelapDamage;
}

bool USkillBase::CheckCurCoolDown()
{
	if (!skillInfoManager)
	{
		return false;
	}

	int coolDown = skillInfoManager->GetCurCoolDown();
	if (coolDown <= 0)
	{
		return true;
	}
	return false;
}

int32 USkillBase::CountCurCoolDown()
{
	if (skillInfoManager && skillInfoManager->GetCurCoolDown() > 0)
	{
		skillInfoManager->AddCurCoolDown(-1);

		return skillInfoManager->GetCurCoolDown();
	}

	return 0;
}

