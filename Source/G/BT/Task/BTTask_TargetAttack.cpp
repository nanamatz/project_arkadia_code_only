// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_TargetAttack.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TargetAttack::UBTTask_TargetAttack()
{
	TaskCost = 1;
	NodeName = TEXT("TargetAttack");
}

EBTNodeResult::Type UBTTask_TargetAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	ControllingPawn->GetController()->StopMovement();

	ANonPlayerCharacter* MyNPC = Cast<ANonPlayerCharacter>(ControllingPawn);
	UObject* OTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
	AGCharacter* Target;
	if (OTarget)
	{
		Target = Cast<AGCharacter>(OTarget);
		if (MyNPC && !Target->GetBDeath())
		{
			MyNPC->TargetAttack(Target);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
