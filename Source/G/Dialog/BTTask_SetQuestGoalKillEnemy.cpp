// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetQuestGoalKillEnemy.h"
#include "G/NonPlayerCharacter.h"
#include "G/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "G/Dialog/FQuestManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestGoalKillEnemy::UBTTask_SetQuestGoalKillEnemy()
{
	NodeName = TEXT("SetQuestGoalKillEnemy");
}

EBTNodeResult::Type UBTTask_SetQuestGoalKillEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	FName textname = TEXT("QuestHashTag");
	FName boolname = TEXT("QuestGoalClear");
	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FString text = OwnerComp.GetBlackboardComponent()->GetValueAsString(textname);
	int32 index = gin->QuestManager->GetHashTag(text);

	UE_LOG(LogTemp, Log, TEXT("SetQuestKillEnemy(27) : Hash Tag - %s, Index - %d"), *text, index);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolname, false);

	TMap<ANonPlayerCharacter*, int32> MapList;
	ANonPlayerCharacter* pointer;
	for (auto iter : EnemyList)
	{
		pointer = iter.Key.GetDefaultObject();
		MapList.Add(pointer, iter.Value);
	}

	gin->QuestManager->QuestGoalInitialize(index);
	gin->QuestManager->SetQuestGoalKillEnemy(index, MapList);
	return EBTNodeResult::Succeeded;
}
