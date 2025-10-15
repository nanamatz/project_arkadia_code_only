// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UseSkill.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/G.h"
#include "G/GCharacter.h"
#include "G/NPCAIController.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

UBTTask_UseSkill::UBTTask_UseSkill()
{
	NodeName = TEXT("UseSkill");
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AGCharacter* myCharacter = Cast<AGCharacter>(ControllingPawn);

	LOG(Log, "myCharacter Use Skill : %s", *myCharacter->GetCharacterName().ToString());
	myCharacter->GetController()->StopMovement();
	myCharacter->GetSkillSystemComponent()->UseSkill();

	BlackboardComp->SetValueAsBool(ANPCAIController::BSelectSkillKey, false);
	BlackboardComp->SetValueAsBool(ANPCAIController::BUseSkillKey, false);

	int useCost = myCharacter->GetSkillSystemComponent()->GetSelectedSkillSlot()->GetMySkill()->GetSkillInfoManager()->GetCost();
	int curCost = myCharacter->AddCurrentCost(-useCost);

	ANPCAIController* myController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	myController->SetCurCostKey(curCost);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
