// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_FindTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "G/G.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/CombatManager.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindTarget::UBTTask_FindTarget()
{
	NodeName = TEXT("FindSkillTarget");
}


EBTNodeResult::Type UBTTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	switch (TargetType)
	{
	case Close:
		return CloseTypeTask(OwnerComp, NodeMemory);
	case Weak:
		return WeakTypeTask(OwnerComp, NodeMemory);
	default:
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UBTTask_FindTarget::CloseTypeTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(ControllingPawn->GetWorld()));
	AGCharacter* target = nullptr;

	// 자기 위치 업데이트
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANPCAIController::HomePosKey,
	                                                     ControllingPawn->GetActorLocation());
	// 가까운 타겟 찾기
	TArray<AGCharacter*> characters;
	AGCharacter* myCharacter = Cast<AGCharacter>(ControllingPawn);
	float MinDistance = INFINITY;

	// 스킬 타입에 맞는 타겟 찾기
	USkillSlot* selectedSkillSlot = myCharacter->GetSkillSystemComponent()->GetSelectedSkillSlot();
	if (!selectedSkillSlot)
	{
		return EBTNodeResult::Failed;
	}
	USkillBase* mySkill = selectedSkillSlot->GetMySkill();
	ESkillTargetType targetType = mySkill->GetSkillInfoManager()->GetTargetType();

	switch (targetType)
	{
	case ESkillTargetType::PLAYER:
		for (const auto character : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
		{
			characters.Add(character);
		}
		break;
	case ESkillTargetType::NPC:
		for (const auto character : GameInstance->GetCombatManager()->GetCombatArea()->GetEnemyList())
		{
			if (!character->GetCharacterInfoManager()->GetBDeath())
			{
				characters.Add(character);
			}
		}
		break;
	case ESkillTargetType::SELF:
		characters.Add(myCharacter);
		break;
	case ESkillTargetType::ALL:
		for (const auto character : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
		{
			characters.Add(character);
		}
		for (const auto character : GameInstance->GetCombatManager()->GetCombatArea()->GetEnemyList())
		{
			if (character != myCharacter)
			{
				characters.Add(character);
			}
		}
		break;
	default:
		LOG(Error, "타겟이 설정되어있지 않습니다.");
		break;
	}

	for (const auto character : characters)
	{
		// 플레이어 캐릭터가 있을 때
		if (character && !character->GetBDeath())
		{
			float CurDistance = FVector::Dist(character->GetActorLocation(), myCharacter->GetActorLocation());
			if (MinDistance > CurDistance)
			{
				target = character;
				MinDistance = CurDistance;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, nullptr);
	if(target)
		myCharacter->GetSkillSystemComponent()->SetSkillTarget(target);

	if (myCharacter && target)
	{
		// 타겟 업데이트
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, target);
		// 도착 지점 업데이트
		OwnerComp.GetBlackboardComponent()->
		          SetValueAsVector(ANPCAIController::TargetPosKey, target->GetActorLocation());
		// 타겟 거리 업데이트
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ANPCAIController::TargetDistance,
		                                                    myCharacter->GetDistanceTo(target) / 100.0f);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_FindTarget::WeakTypeTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}

void UBTTask_FindTarget::AddTargetList(TArray<AGCharacter*>& targetList, TArray<AGCharacter*>& addList)
{
	for (AGCharacter* target : addList)
	{
		if (!target->GetCharacterInfoManager()->GetBDeath())
		{
			targetList.Add(target);
		}
	}
}
