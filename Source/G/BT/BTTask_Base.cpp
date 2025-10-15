// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/BTTask_Base.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"


EBTNodeResult::Type UBTTask_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!UseCost(OwnerComp, NodeMemory, TaskCost))
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

bool UBTTask_Base::UseCost(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, int& useCost)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ANPCAIController* myController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	ANonPlayerCharacter* myCharacter = Cast<ANonPlayerCharacter>(ControllingPawn);

	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	int curCost = BlackboardComp->GetValueAsInt(ANPCAIController::CurCostKey);


	if (curCost - useCost < 0)
	{
		return false;
	}

	myCharacter->AddCurrentCost(-useCost);
	myController->SetCurCostKey(curCost - useCost);

	return true;
}
