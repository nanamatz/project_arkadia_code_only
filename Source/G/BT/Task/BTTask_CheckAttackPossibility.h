// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_CheckAttackPossibility.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_CheckAttackPossibility : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UBTTask_CheckAttackPossibility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
