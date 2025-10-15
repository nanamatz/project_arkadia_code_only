// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Enhance.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_Enhance : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Enhance();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
