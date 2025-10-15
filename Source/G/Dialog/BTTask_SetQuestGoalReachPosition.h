// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetQuestGoalReachPosition.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetQuestGoalReachPosition : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_SetQuestGoalReachPosition();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
