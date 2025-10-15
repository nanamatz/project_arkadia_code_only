// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AddExp.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_AddExp : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 Exp;

	UBTTask_AddExp();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
