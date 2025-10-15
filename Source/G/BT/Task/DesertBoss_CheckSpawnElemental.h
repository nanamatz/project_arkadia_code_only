// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "DesertBoss_CheckSpawnElemental.generated.h"

/**
 * 
 */
UCLASS()
class G_API UDesertBoss_CheckSpawnElemental : public UBTTask_Base
{
	GENERATED_BODY()

	UDesertBoss_CheckSpawnElemental();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
