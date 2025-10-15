// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_DesertBoss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AAIController_DesertBoss::SpawnSandElemental(TEXT("SpawnSandElemental"));

AAIController_DesertBoss::AAIController_DesertBoss()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Desert/Boss/BB_DesertBoss.BB_DesertBoss'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Desert/Boss/BT_DesertBoss.BT_DesertBoss'"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}

void AAIController_DesertBoss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Blackboard->SetValueAsBool(SpawnSandElemental, false);
}
