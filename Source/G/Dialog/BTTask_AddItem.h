// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AddItem.generated.h"

/**
 *
 */
UCLASS()
class G_API UBTTask_AddItem : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemBase> target_item;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 item_amount;

	UBTTask_AddItem();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
