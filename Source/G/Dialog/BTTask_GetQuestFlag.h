// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetQuestFlag.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_GetQuestFlag : public UBTTaskNode
{
	GENERATED_BODY()
	FString text;
	int32 getnumber;

public:
	UBTTask_GetQuestFlag();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
