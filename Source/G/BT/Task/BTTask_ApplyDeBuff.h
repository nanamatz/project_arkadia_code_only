// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_ApplyDeBuff.generated.h"

/**
 * 
 */

UENUM()
enum class DEBUFFTYPE_WRAPPER : uint8
{
	SHIELDDECREASE UMETA(DisplayName = "SHIELDDECREASE"), //취약
	DAMAGEDECREASE UMETA(DisplayName = "DAMAGEDECREASE"), //어지럼증
	HEALINGDECREASE UMETA(DisplayName = "HEALINGDECREASE"), //	저주
	STUN UMETA(DisplayName = "STUN"), //기절
	APPLYPIERCEDAMAGE UMETA(DisplayName = "APPLYPIERCEDAMAGE") //출혈
};

UCLASS()
class G_API UBTTask_ApplyDeBuff : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	int32 turnCount = 1;

	UPROPERTY(EditAnyWhere)
	float amount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	DEBUFFTYPE_WRAPPER debufftype;

	UBTTask_ApplyDeBuff();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
