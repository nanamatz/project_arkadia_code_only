// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ColleaguesRecruitment.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBTTask_ColleaguesRecruitment : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class AGGameMode* GameMode;

public:
	UPROPERTY(EditAnyWhere)
	int NPC_num;

	UBTTask_ColleaguesRecruitment();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION(BlueprintCallable)
	void RecruitmentNPC(int n);
};
