// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Service/BTS_CostCheck.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"

UBTS_CostCheck::UBTS_CostCheck()
{
	NodeName = "BTS_CostCheck";
	bNotifyTick = true;
}

void UBTS_CostCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ANonPlayerCharacter* myCharacter = Cast<ANonPlayerCharacter>(controllingPawn);

	// 현재 코스트
	int CurCost = myCharacter->GetCurrentCost();
	float moveCost = BlackboardComp->GetValueAsFloat(ANPCAIController::MoveCostKey);

	UE_LOG(LogTemp, Warning, TEXT("cost : %d, move cost : %.2f turn end"), CurCost, moveCost);
	if (CurCost <= -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("cost : %d, move cost : %.2f turn end"), CurCost, moveCost);


		OwnerComp.RequestExecution(EBTNodeResult::Failed);
	}

	OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
}
