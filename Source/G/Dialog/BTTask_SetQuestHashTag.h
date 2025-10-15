// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetQuestHashTag.generated.h"

/**
 * 
 */

UCLASS()
class G_API UBTTask_SetQuestHashTag : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	FString hashtag;

	UBTTask_SetQuestHashTag();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
