// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckQuestGoalClear.h"
#include "G/Item/ItemBase.h"
#include "G/NonPlayerCharacter.h"
#include "G/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "G/Dialog/FQuestManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/Manager/InventoryManager.h"

UBTTask_CheckQuestGoalClear::UBTTask_CheckQuestGoalClear()
{
	NodeName = TEXT("CheckQuestGoalClear");
}

EBTNodeResult::Type UBTTask_CheckQuestGoalClear::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FName textname = TEXT("QuestHashTag");
	FName boolname = TEXT("QuestGoalClear");
	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FString text = OwnerComp.GetBlackboardComponent()->GetValueAsString(textname);
	int32 index = gin->QuestManager->GetHashTag(text);

	bool isClear = gin->QuestManager->CheckQuestGoalComplete(index);

	if (isClear)
	{
		UQuestInfo* Quest = gin->QuestManager->GetQuestList()[index];
		for (auto iter : Quest->RequiredItemsList)
		{
			gin->inventory->DeleteItem(
				gin->inventory->GetItemByName( FText::FromString(iter.Key)), iter.Value);
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolname, isClear);
	return EBTNodeResult::Succeeded;
}
