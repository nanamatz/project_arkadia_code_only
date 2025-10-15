// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetQuestAutomationReward.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SetQuestAutomationReward : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	FString HashTag;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 State = -1;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<class UItemBase>, int32> target_item;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 Gold;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 Exp;

	UBTTask_SetQuestAutomationReward();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
