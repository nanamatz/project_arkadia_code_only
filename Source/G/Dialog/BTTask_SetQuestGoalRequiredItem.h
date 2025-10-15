// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetQuestGoalRequiredItem.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetQuestGoalRequiredItem : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	TMap<TSubclassOf<class UItemBase>, int32> ItemList;

	UBTTask_SetQuestGoalRequiredItem();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
