// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetQuestFlag.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetQuestFlag : public UBTTaskNode
{
	GENERATED_BODY()
	FString text;

public:
	UPROPERTY(EditAnyWhere)
	int setnumber;

	UPROPERTY(EditAnyWhere)
	bool isAccept;

	UPROPERTY(EditAnywhere)
	bool isComplete;

	UBTTask_SetQuestFlag();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
