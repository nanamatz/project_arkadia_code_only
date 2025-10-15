// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/AIController_BuffBlue.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/NPC/NPC_BuffGreen.h"

AAIController_BuffBlue::AAIController_BuffBlue()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Minion/Buf_Blue/BB_Buff_Blue.BB_Buff_Blue'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Minion/Buf_Blue/BT_Buff_Blue.BT_Buff_Blue'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
