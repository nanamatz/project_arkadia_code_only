// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/BTTask_SetQuestGoalReachPosition.h"
#include "G/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "G/Dialog/FQuestManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestGoalReachPosition::UBTTask_SetQuestGoalReachPosition()
{
	NodeName = TEXT("SetQuestGoalReachPosition");
}

EBTNodeResult::Type UBTTask_SetQuestGoalReachPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	FName textname = TEXT("QuestHashTag");
	FName boolname = TEXT("QuestGoalClear");
	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FString text = OwnerComp.GetBlackboardComponent()->GetValueAsString(textname);
	int32 index = gin->QuestManager->GetHashTag(text);

	UE_LOG(LogTemp, Log, TEXT("SetQuestReachPositin(26) : Hash Tag - %s, Index - %d"), *text, index);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolname, false);

	gin->QuestManager->QuestGoalInitialize(index);
	gin->QuestManager->SetQuestGoalReachPosition(index);
	return EBTNodeResult::Succeeded;
}