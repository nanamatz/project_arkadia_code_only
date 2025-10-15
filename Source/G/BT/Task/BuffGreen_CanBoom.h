// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BuffGreen_CanBoom.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBuffGreen_CanBoom : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UBuffGreen_CanBoom();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
