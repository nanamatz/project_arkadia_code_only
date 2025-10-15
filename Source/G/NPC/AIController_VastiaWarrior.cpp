#include "AIController_VastiaWarrior.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AAIController_VastiaWarrior::AAIController_VastiaWarrior()
{
	// 해당 NPC에 맞는 BB, BT 설정하기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/CombatSystem/NPC/Vastia/Warrior/BB_VastiaWarrior.BB_VastiaWarrior'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/CombatSystem/NPC/Vastia/Warrior/BT_VastiaWarrior.BT_VastiaWarrior'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}
