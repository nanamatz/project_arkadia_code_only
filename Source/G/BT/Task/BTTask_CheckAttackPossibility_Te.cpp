// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckAttackPossibility_Te.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_CheckAttackPossibility_Te::UBTTask_CheckAttackPossibility_Te()
{
	NodeName = TEXT("CheckAttackPossibility_designation");
}

EBTNodeResult::Type UBTTask_CheckAttackPossibility_Te::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			if (MyNPC->CheckAttackPossibility(Target, Radious))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::CanTargetAttackKey, true);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::CanTargetAttackKey, false);
	return EBTNodeResult::Succeeded;
}
