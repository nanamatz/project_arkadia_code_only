// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/AIController_BuffGreen.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/NPC/NPC_BuffGreen.h"

// 필요한 키 값
const FName AAIController_BuffGreen::CanBoomKey(TEXT("CanBoom"));

AAIController_BuffGreen::AAIController_BuffGreen()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("/Game/CombatSystem/NPC/Minion/Buff_Green/BB_Buff_Green"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("/Game/CombatSystem/NPC/Minion/Buff_Green/BT_Buff_Green"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}

void AAIController_BuffGreen::SkillBoom()
{
	ANPC_BuffGreen* MyBuffGreen = Cast<ANPC_BuffGreen>(GetPawn());
	if (MyBuffGreen)
	{
		MyBuffGreen->Death();
	}
}
