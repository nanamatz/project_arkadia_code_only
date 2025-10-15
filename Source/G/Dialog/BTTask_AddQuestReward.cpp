// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AddQuestReward.h"

#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Item/ItemBase.h"
#include "G/Manager/InventoryManager.h"
#include "G/Manager/InGameDataManager.h"
#include "G/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AddQuestReward::UBTTask_AddQuestReward()
{
	NodeName = TEXT("AddQuestReward");
}

EBTNodeResult::Type UBTTask_AddQuestReward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	UItemBase* RewardItem;

	for (auto iter : target_item)
	{
		RewardItem = NewObject<UItemBase>(GameInstance, iter.Key.Get());

		for (int i = 0; i < iter.Value; i++)
		{
			if (RewardItem)
			{
				GameInstance->inventory->AddItem(RewardItem);
			}
		}
	}

	GameInstance->inventory->AddGold(Gold);

	for (auto iter : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
	{
		iter->GetCharacterInfoManager()->AddCurrentExp(Exp);
	}

	return EBTNodeResult::Succeeded;
}
