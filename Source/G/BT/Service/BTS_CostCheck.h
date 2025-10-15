// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CostCheck.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTS_CostCheck : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CostCheck();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
