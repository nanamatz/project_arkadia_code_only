// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetQuestHashTag.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestHashTag::UBTTask_SetQuestHashTag()
{
}

EBTNodeResult::Type UBTTask_SetQuestHashTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	FName text = "QuestHashTag";
	OwnerComp.GetBlackboardComponent()->SetValueAsString(text, hashtag);

	return EBTNodeResult::Succeeded;
}
