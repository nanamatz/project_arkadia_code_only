// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_FindPatrolPos.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "G/CombatArea.h"
#include "G/GCharacter.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation NextPatrol;

	if (IsCombatAreaCenter)
	{
		UObject* obj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::CombatAreaKey);
		if(obj){
			ACombatArea* combatArea = Cast<ACombatArea>(obj);
			NavSystem->GetRandomPointInNavigableRadius(combatArea->GetActorLocation(), PatrolRange, NextPatrol);
		}
	}
	else
	{
		FVector originPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ANPCAIController::OriginPosKey);
		NavSystem->GetRandomPointInNavigableRadius(originPos, PatrolRange, NextPatrol);
	}
	auto myCharacter = Cast<AGCharacter>(ControllingPawn);
	if(myCharacter->GetBDeath())
	{
		return EBTNodeResult::Failed;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANPCAIController::PatrolPosKey, NextPatrol.Location);
	return EBTNodeResult::Succeeded;
}
