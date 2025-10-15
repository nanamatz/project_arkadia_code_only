// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BuffGreen_CanBoom.h"
#include "G/NPC/AIController_BuffGreen.h"
#include "BehaviorTree/BlackboardComponent.h"

UBuffGreen_CanBoom::UBuffGreen_CanBoom()
{
	NodeName = TEXT("CanBoom");
}

EBTNodeResult::Type UBuffGreen_CanBoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 다음 턴에 터질 수 있게 True로 변경
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAIController_BuffGreen::CanBoomKey, true);

	return EBTNodeResult::Succeeded;
}
