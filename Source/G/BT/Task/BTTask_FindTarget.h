// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_FindTarget.generated.h"

/**
 * 
 */
UENUM()
enum ETargetType
{
	Close,
	Weak,
};

UCLASS()
class G_API UBTTask_FindTarget : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UBTTask_FindTarget();

	UPROPERTY(EditAnywhere, Category=Task)
	TEnumAsByte<ETargetType> TargetType;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type CloseTypeTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	EBTNodeResult::Type WeakTypeTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void AddTargetList(TArray<class AGCharacter*>& targetList, TArray<class AGCharacter*>& addList);
};
