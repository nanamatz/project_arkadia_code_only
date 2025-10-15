// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_FindPatrolPos : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPos();

	UPROPERTY(EditAnywhere, Category=Task)
	bool IsCombatAreaCenter = true;

	UPROPERTY(EditAnywhere, Category=Task)
	float PatrolRange = 500.0f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
