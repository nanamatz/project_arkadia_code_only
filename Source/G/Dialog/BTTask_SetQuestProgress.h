// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/Dialog/FQuestManager.h"
#include "BTTask_SetQuestProgress.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetQuestProgress : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString HashTag;

	UPROPERTY(EditAnywhere)
	EQuestProgress Progress;

	UPROPERTY(EditAnywhere)
	int32 State = -1;

	UBTTask_SetQuestProgress();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
