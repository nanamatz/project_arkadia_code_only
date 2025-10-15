// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Task/BTTask_TurnEnd.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "G/CombatArea.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "G/GGameInstance.h"
#include "G/Manager/CombatManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_TurnEnd::UBTTask_TurnEnd()
{
	NodeName = TEXT("TurnEnd");
}

EBTNodeResult::Type UBTTask_TurnEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}


	ANonPlayerCharacter* NPC = Cast<ANonPlayerCharacter>(ControllingPawn);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (NPC)
	{
		BlackboardComp->SetValueAsBool(ANPCAIController::BSelectSkillKey, false);
		BlackboardComp->SetValueAsBool(ANPCAIController::BUseSkillKey, false);
		NPC->GetController()->StopMovement();
		GameInstance->GetCombatManager()->TurnEnd();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
