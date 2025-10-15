// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_SkillBoom.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <G/NPC/NPC_BuffGreen.h>

UBTTask_SkillBoom::UBTTask_SkillBoom()
{
	NodeName = TEXT("SkillBoom");
}

EBTNodeResult::Type UBTTask_SkillBoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ANPC_BuffGreen* MyBuffGreen = Cast<ANPC_BuffGreen>(ControllingPawn);
	if (MyBuffGreen)
	{
		MyBuffGreen->Death();
	}

	return EBTNodeResult::Succeeded;
}
