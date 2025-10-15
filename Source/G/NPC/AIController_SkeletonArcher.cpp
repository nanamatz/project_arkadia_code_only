// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/AIController_SkeletonArcher.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIController_SkeletonArcher::AAIController_SkeletonArcher()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Skeleton/Archer/BB_SkeletonArcher.BB_SkeletonArcher'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Skeleton/Archer/BT_SkeletonArcher.BT_SkeletonArcher'"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
