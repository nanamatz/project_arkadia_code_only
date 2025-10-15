// Fill out your copyright notice in the Description page of Project Settings.

#include "G/Dialog/BTTask_SetQuestAutomationReward.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Item/ItemBase.h"
#include "G/Dialog/FQuestManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestAutomationReward::UBTTask_SetQuestAutomationReward()
{
	NodeName = TEXT("SetQuestAutomationReward");
}

EBTNodeResult::Type UBTTask_SetQuestAutomationReward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	int32 index = GameInstance->QuestManager->GetHashTag(HashTag);
	
	GameInstance->QuestManager->GetQuestList()[index]->isAutomationSet = true;

	UItemBase* RewardItem;

	TMap<UItemBase*, int32> ItemList;

	for (auto iter : target_item)
	{
		RewardItem = NewObject<UItemBase>(GameInstance, iter.Key.Get());

		if (RewardItem)
		{
			ItemList.Add(RewardItem, iter.Value);
		}
	}

	GameInstance->QuestManager->SetQuestAutomationComplete(index, ItemList, Gold, Exp, State);

	return EBTNodeResult::Succeeded;
}
