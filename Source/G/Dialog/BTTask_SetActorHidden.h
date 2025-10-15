// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetActorHidden.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetActorHidden : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	int NpcNumber;

	UPROPERTY(EditAnyWhere)
	bool Visible;

	UBTTask_SetActorHidden();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
