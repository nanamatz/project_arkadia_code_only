// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetQuestGoalRequiredItem.h"
#include "G/Item/ItemBase.h"
#include "G/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "G/Dialog/FQuestManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestGoalRequiredItem::UBTTask_SetQuestGoalRequiredItem()
{
	NodeName = TEXT("SetQuestGoalRequiredItem");
}

EBTNodeResult::Type UBTTask_SetQuestGoalRequiredItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//FName valuename = TEXT("QuestState");
	FName textname = TEXT("QuestHashTag");
	FName boolname = TEXT("QuestGoalClear");
	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FString text = OwnerComp.GetBlackboardComponent()->GetValueAsString(textname);
	int32 n = gin->QuestManager->GetHashTag(text);

	UE_LOG(LogTemp, Log, TEXT("SetQuestRequiredItem(22) : Hash Tag - %s, Index - %d"), *text, n);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolname, false);

	TMap<UItemBase*, int32> MapList;
	UItemBase* pointer;
	for (auto iter : ItemList)
	{
		pointer = iter.Key.GetDefaultObject();
		MapList.Add(pointer, iter.Value);
	}

	gin->QuestManager->QuestGoalInitialize(n);
	gin->QuestManager->SetQuestGoalRequiredItem(n, MapList);

	return EBTNodeResult::Succeeded;
}
