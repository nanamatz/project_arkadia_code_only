#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "G/GCharacter.h"
#include "BTTask_SelectSkill.generated.h"

UCLASS()
class UBTTask_SelectSkill : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SelectSkill();

	// [a, b] -> a는 스킬 번호, b는 우선순위, 같은 경우 랜덤
	UPROPERTY(EditAnywhere, Category="Skill Prioirty")
	TMap<int32, int32> _prioritySkillMap;

	UPROPERTY(EditAnywhere, Category="Skill Prioirty")
	bool IsRandom = false;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	TArray<int32> SelectGetSkillList(AGCharacter* AGCharacter);
};
