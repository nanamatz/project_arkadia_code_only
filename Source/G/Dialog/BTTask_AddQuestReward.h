// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AddQuestReward.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_AddQuestReward : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<class UItemBase>, int32> target_item;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 Gold;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 Exp;

	UBTTask_AddQuestReward();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
