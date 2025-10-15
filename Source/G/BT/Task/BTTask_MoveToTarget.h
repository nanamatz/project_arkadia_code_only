// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * 타겟 위치로 이동
 */
UCLASS()
class G_API UBTTask_MoveToTarget : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UBTTask_MoveToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
