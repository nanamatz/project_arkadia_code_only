// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_SpawnCharacter.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_SpawnCharacter : public UBTTask_Base
{
	GENERATED_BODY()

	int32 EnemyNumber = -1;

public:
	UPROPERTY(EditAnyWhere)
	TSubclassOf<class ANonPlayerCharacter> spawnTarget;

	UBTTask_SpawnCharacter();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
