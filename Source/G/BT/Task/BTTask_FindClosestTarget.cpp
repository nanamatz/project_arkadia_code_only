// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_FindClosestTarget.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "G/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

UBTTask_FindClosestTarget::UBTTask_FindClosestTarget()
{
	NodeName = TEXT("FindClosestTarget");
}

EBTNodeResult::Type UBTTask_FindClosestTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 자기 위치 업데이트
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANPCAIController::HomePosKey,
	                                                     ControllingPawn->GetActorLocation());


	// 가까운 타겟 찾기
	ANonPlayerCharacter* MyNPC = Cast<ANonPlayerCharacter>(ControllingPawn);
	APlayerCharacter* Target = MyNPC->FindClosestTarget();
	if (MyNPC && Target)
	{
		MyNPC->GetSkillSystemComponent()->SetSkillTarget(Target);
		// 타겟 업데이트
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, Target);
		// 도착 지점 업데이트
		OwnerComp.GetBlackboardComponent()->
		          SetValueAsVector(ANPCAIController::TargetPosKey, Target->GetActorLocation());
		// 타겟 거리 업데이트
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ANPCAIController::TargetDistance,
		                                                    MyNPC->GetDistanceTo(Target) / 100.0f);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}
