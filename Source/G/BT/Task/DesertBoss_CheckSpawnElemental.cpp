// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertBoss_CheckSpawnElemental.h"
#include "G/NPC/AIController_DesertBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

UDesertBoss_CheckSpawnElemental::UDesertBoss_CheckSpawnElemental()
{
	NodeName = TEXT("CheckSpawnElemental");
}

EBTNodeResult::Type UDesertBoss_CheckSpawnElemental::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAIController_DesertBoss::SpawnSandElemental, true);
	return EBTNodeResult::Succeeded;
}
