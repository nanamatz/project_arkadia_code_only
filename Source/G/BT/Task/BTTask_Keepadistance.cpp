// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_Keepadistance.h"
#include "G/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include <G/NonPlayerCharacter.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>

#include "G/CharacterInfoComponent.h"
#include "G/G.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

UBTTask_Keepadistance::UBTTask_Keepadistance()
{
	NodeName = TEXT("Keep a Distance");
}

EBTNodeResult::Type UBTTask_Keepadistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AGCharacter* NPC = Cast<AGCharacter>(ControllingPawn);
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ANPCAIController::HomePosKey);
	FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ANPCAIController::TargetPosKey);
	float TargetDistance = FVector::Dist(Origin, TargetPos);

	FVector velocity = ControllingPawn->GetVelocity();

	if (velocity.IsNearlyZero())
	{
		FNavLocation NextPatrol;
		NextPatrol.Location = Origin;
		USkillBase* skill = NPC->GetSkillSystemComponent()->GetSelectedSkillSlot()->GetMySkill();
		if (skill)
		{
			const float attackRange = skill->GetSkillInfoManager()->GetCharacterRange();

			if (NavSystem->GetRandomPointInNavigableRadius(Origin, attackRange, NextPatrol))
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(ControllingPawn->GetController(), NextPatrol);
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
