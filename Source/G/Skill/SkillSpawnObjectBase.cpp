// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SkillSpawnObjectBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <G/GCharacter.h>
#include <G/PlayerCharacter.h>
#include <G/NonPlayerCharacter.h>
#include <G/GGameMode.h>
#include <G/CombatArea.h>
#include <Kismet/GameplayStatics.h>

#include "SkillBase.h"
#include "SkillInfoManager.h"
#include "G/Manager/CombatManager.h"

// Sets default values
ASkillSpawnObjectBase::ASkillSpawnObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	areaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("areaCollision"));
	SetRootComponent(areaCollision);
	//holyAreaCollision->SetupAttachment(RootComponent);
	areaCollision->SetCollisionProfileName(TEXT("SkillCollision"));
	areaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	areaCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkillSpawnObjectBase::OnCollisionBeginOverlap);
	areaCollision->OnComponentEndOverlap.AddDynamic(this, &ASkillSpawnObjectBase::OnCollisionEndOverlap);
}

void ASkillSpawnObjectBase::Init(USkillBase* _mySkill, AGCharacter* _myCharacter)
{
	bInit = true;
	myCharacter = _myCharacter;
	mySkill = _mySkill;

	for(auto attack : mySkill->GetSkillInfoManager()->GetAttackList())
	{
		if(attack.Duration > 0)
		{
			durationTurn = 	attack.Duration + 1;
			bCountTurn = false;
			attakcInfo = attack;
			break;
		}
	}
	
	float localScale = _mySkill->GetSkillInfoManager()->GetMouseRange() / 100.0f;
	this->SetActorScale3D(FVector(localScale, localScale, localScale));

	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));
	
}

// Called every frame
void ASkillSpawnObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bInit)
		return;
	TurnCount();
	// 이동 감지 시 데미지
	MoveCharacterAttack();
	// 턴 시작 시 데미지
}

void ASkillSpawnObjectBase::TurnCount()
{
	if (!GameInstance->GetCombatManager()->CheckCombatTure())
	{
		this->Destroy();
	}
	ACombatArea* combatArea = GameInstance->GetCombatManager()->GetCombatArea();
	if (!combatArea)
	{
		return;
	}
	AGCharacter* curTurnCharacter = GameInstance->GetCombatManager()->GetCurTrunCharacter();
	if (!curTurnCharacter)
	{
		return;
	}
	if (curTurnCharacter == myCharacter)
	{
		if (!bCountTurn)
		{
			durationTurn--;
			bCountTurn = true;
			if (durationTurn == 0)
			{
				this->Destroy();
			}
		}
	}
	else
	{
		bCountTurn = false;
	}
}

void ASkillSpawnObjectBase::MoveCharacterAttack()
{
	ACombatArea* combatArea = GameInstance->GetCombatManager()->GetCombatArea();
	if (!combatArea)
	{
		return;
	}
	AGCharacter* curTurnCharacter = GameInstance->GetCombatManager()->GetCurTrunCharacter();
	if (!curTurnCharacter)
	{
		return;
	}
	for (auto& elem : attackAreaCharacterList)
	{
		AGCharacter* character = elem.Key;
		if (curTurnCharacter && !(character == curTurnCharacter))
		{
			elem.Value = false;
		}
		if (character->GetMovementComponent()->Velocity != FVector::ZeroVector)
		{
			if (elem.Value == false)
			{
				mySkill->TargetAttack(character);
				elem.Value = true;
			}
		}
	}
}


void ASkillSpawnObjectBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bInit)
		return;
	
	//UE_LOG(LogTemp, Warning, TEXT("OnAreaCollisionBeginOverlap"));
	// Check if the overlapped actor is of the desired type (AGCharacter)
	if (OtherActor && OtherActor->IsA(AGCharacter::StaticClass()))
	{
		// Handle the overlapped actor
		AGCharacter* OverlappedCharacter = Cast<AGCharacter>(OtherActor);
		if (OverlappedCharacter && !OverlappedCharacter->GetBDeath() && !attackAreaCharacterList.Contains(OverlappedCharacter))
		{
			const ESkillTargetType type = mySkill->GetSkillInfoManager()->GetTargetType();
			const ECharacterName characterEName = OverlappedCharacter->GetECharacterName();
			switch (type)
			{
			case ESkillTargetType::ALL:
				attackAreaCharacterList.Add(OverlappedCharacter, false);
				break;
			case ESkillTargetType::PLAYER:
				if(characterEName != ECharacterName::NonPlayerCharacter)
				{
					attackAreaCharacterList.Add(OverlappedCharacter, false);
				}
				break;
			case ESkillTargetType::NPC:
				if(characterEName == ECharacterName::NonPlayerCharacter)
				{
					attackAreaCharacterList.Add(OverlappedCharacter, false);
				}
				break;
			case ESkillTargetType::SELF:
				if(characterEName == myCharacter->GetECharacterName())
				{
					attackAreaCharacterList.Add(OverlappedCharacter, false);
				}
				break;
			default:
				break;
			}
			// Add the overlapped AGCharacter to the array
			//UE_LOG(LogTemp, Warning, TEXT("OnAreaCollisionBeginOverlap Name : %s"), *(OtherActor->GetName()));
			
		}
	}
}

// 범위 밖으로 나가면 지우기
void ASkillSpawnObjectBase::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!bInit)
		return;
	
	//UE_LOG(LogTemp, Warning, TEXT("Out OnAreaCollisionEndOverlap"));
	// Check if the overlapped actor is of the desired type (AGCharacter)
	if (OtherActor && OtherActor->IsA(AGCharacter::StaticClass()) && OtherActor != this)
	{
		// Handle the overlapped actor
		AGCharacter* OverlappedCharacter = Cast<AGCharacter>(OtherActor);
		if (OverlappedCharacter)
		{
			attackAreaCharacterList.Remove(OverlappedCharacter);
			//UE_LOG(LogTemp, Warning, TEXT("Out OnAreaCollisionEndOverlap Name : %s"), *(OtherActor->GetName()));
		}
	}
}
