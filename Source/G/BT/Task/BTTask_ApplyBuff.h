// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/BT/BTTask_Base.h"
#include "BTTask_ApplyBuff.generated.h"

/**
 * 
 */

UENUM()
enum class BUFFTYPE_WRAPPER : uint8
{
	SHIELDINCREASE UMETA(DisplayName = "견고함"), // 견고함
	INSIGHTINCREASE UMETA(DisplayName = "총명"), // 총명
	COSTINCREASE UMETA(DisplayName = "축복"), // 축복
	STRENGTHINCREASE UMETA(DisplayName = "분노"), // 분노
	LUCKYINCREASE UMETA(DisplayName = "가호"), // 가호
	MOVECOSTINCREASE UMETA(DisplayName = "기민함") // 기민함
};


UCLASS()
class G_API UBTTask_ApplyBuff : public UBTTask_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	int32 turnCount = 1;

	UPROPERTY(EditAnyWhere)
	float amount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BUFFTYPE_WRAPPER bufftype;

	UBTTask_ApplyBuff();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
