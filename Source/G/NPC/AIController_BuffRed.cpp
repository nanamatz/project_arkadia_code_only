// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/AIController_BuffRed.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/NPC/NPC_BuffGreen.h"

AAIController_BuffRed::AAIController_BuffRed()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("/Game/CombatSystem/NPC/Minion/Buff_Red/BB_Buff_Red"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("/Game/CombatSystem/NPC/Minion/Buff_Red/BT_Buff_Red"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
