// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckSkillPossibility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/GCharacter.h"
#include "G/NPCAIController.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

UBTTask_CheckSkillPossibility::UBTTask_CheckSkillPossibility()
{
	NodeName = "CheckSkillPossibility";
}

EBTNodeResult::Type UBTTask_CheckSkillPossibility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AGCharacter* myCharacter = Cast<AGCharacter>(controllingPawn);

	if (BlackboardComp->GetValueAsBool(ANPCAIController::BSelectSkillKey))
	{
		if (myCharacter->GetSkillSystemComponent()->CheckSkillPossibility())
		{
			BlackboardComp->SetValueAsBool(ANPCAIController::BUseSkillKey, true);
		}
		else
		{
			BlackboardComp->SetValueAsBool(ANPCAIController::BUseSkillKey, false);
		}
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
