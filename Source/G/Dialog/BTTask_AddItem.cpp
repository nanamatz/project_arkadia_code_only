// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AddItem.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Item/ArmorItemBase.h"
#include "G/Item/WeaponItemBase.h"
#include "G/Manager/InventoryManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/Util/Define.h"


UBTTask_AddItem::UBTTask_AddItem()
{
	NodeName = TEXT("AddItem");
}

EBTNodeResult::Type UBTTask_AddItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 게임 인스턴스 저장
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	UItemBase* RewardItem = NewObject<UItemBase>(GameInstance, target_item.Get());

	for (int i = 0; i < item_amount; i++)
	{
		if (RewardItem)
		{
			GameInstance->inventory->AddItem(RewardItem);
		}
	}

	return EBTNodeResult::Succeeded;
}
