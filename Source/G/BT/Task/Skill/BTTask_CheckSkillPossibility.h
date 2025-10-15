// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckSkillPossibility.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_CheckSkillPossibility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckSkillPossibility();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
