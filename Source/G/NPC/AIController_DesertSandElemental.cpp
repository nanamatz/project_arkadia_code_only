// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_DesertSandElemental.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIController_DesertSandElemental::AAIController_DesertSandElemental()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT(
			"BlackboardData'/Game/CombatSystem/NPC/Desert/SandElemental/BB_DesertSandElemental.BB_DesertSandElemental'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT(
			"BehaviorTree'/Game/CombatSystem/NPC/Desert/SandElemental/BT_DesertSandElemental.BT_DesertSandElemental'"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
