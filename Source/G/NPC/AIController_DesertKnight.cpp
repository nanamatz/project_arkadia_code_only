// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_DesertKnight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIController_DesertKnight::AAIController_DesertKnight()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Desert/Knight/BB_DesertKnight.BB_DesertKnight'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Desert/Knight/BT_DesertKnight.BT_DesertKnight'"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
