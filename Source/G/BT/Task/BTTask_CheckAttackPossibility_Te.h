// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_CheckAttackPossibility_Te.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_CheckAttackPossibility_Te : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	float Radious = 0.0f;

	UBTTask_CheckAttackPossibility_Te();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
