// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_Keepadistance.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_Keepadistance : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UBTTask_Keepadistance();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
