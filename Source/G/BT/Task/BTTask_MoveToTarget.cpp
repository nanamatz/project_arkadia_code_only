// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_MoveToTarget.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <G/NonPlayerCharacter.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
	NodeName = TEXT("MoveToTarget");
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

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

	FVector homePos = BlackboardComp->GetValueAsVector(ANPCAIController::HomePosKey);
	FVector curPos = ControllingPawn->GetActorLocation();
	FVector targetPos = BlackboardComp->GetValueAsVector(ANPCAIController::TargetPosKey);
	float curMoveCost = BlackboardComp->GetValueAsFloat(ANPCAIController::MoveCostKey);

	float MoveDis = FVector::Dist(homePos, curPos);

	if (curMoveCost <= 0.0f)
	{
		int useCost = 1;
		if (UseCost(OwnerComp, NodeMemory, useCost))
		{
			myCharacter->AddMoveCost(useCost);
			int moveCost = myCharacter->GetMoveCost();
			myController->SetMoveCostKey(moveCost);
			return EBTNodeResult::Failed;
		}
	}

	myController->AddMoveCost(-MoveDis);
	myCharacter->UseMoveCost(MoveDis);

	BlackboardComp->SetValueAsVector(ANPCAIController::HomePosKey, curPos);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(ControllingPawn->GetController(), targetPos);

	return EBTNodeResult::Failed;
}
