// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_MoveTo.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTS_MoveTo : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_MoveTo();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
