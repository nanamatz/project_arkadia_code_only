// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_CheckAttackPossibility.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckAttackPossibility::UBTTask_CheckAttackPossibility()
{
	NodeName = TEXT("CheckAttackPossibility");
}

EBTNodeResult::Type UBTTask_CheckAttackPossibility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ANonPlayerCharacter* MyNPC = Cast<ANonPlayerCharacter>(ControllingPawn);
	UObject* OTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
	AGCharacter* Target;
	if (OTarget)
	{
		Target = Cast<AGCharacter>(OTarget);
		if (MyNPC && Target)
		{
			// 임시로 사거리 설정
			if (MyNPC->CheckAttackPossibility(Target, 400.0f))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::CanTargetAttackKey, true);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::CanTargetAttackKey, false);
	return EBTNodeResult::Succeeded;
	//return EBTNodeResult::Failed;
}
