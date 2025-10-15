// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_KillCrab.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AAIController_KillCrab::AAIController_KillCrab()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Neutral/Monster/KillCrab/BB_KillCrab.BB_KillCrab'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Neutral/Monster/KillCrab/BT_KillCrab.BT_KillCrab'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
