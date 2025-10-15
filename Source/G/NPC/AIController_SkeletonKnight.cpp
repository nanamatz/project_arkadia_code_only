// Fill out your copyright notice in the Description page of Project Settings.

#include "G/NPC/AIController_SkeletonKnight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIController_SkeletonKnight::AAIController_SkeletonKnight()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Skeleton/Knight/BB_SkeletonKnight.BB_SkeletonKnight'"));
	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/CombatSystem/NPC/BB_DefaultNPC_Long.BB_DefaultNPC_Long'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Skeleton/Knight/BT_SkeletonKnight.BT_SkeletonKnight'"));
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/CombatSystem/NPC/BT_DefaultNPC_Long.BT_DefaultNPC_Long'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
