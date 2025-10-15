// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetQuestGoalKillEnemy.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetQuestGoalKillEnemy : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	TMap<TSubclassOf<class ANonPlayerCharacter>, int32> EnemyList;

	UBTTask_SetQuestGoalKillEnemy();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
