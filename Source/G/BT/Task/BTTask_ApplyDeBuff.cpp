// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ApplyDeBuff.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "G/PlayerCharacter.h"
#include "G/Player/CharacterInfoStruct.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTTask_ApplyDeBuff::UBTTask_ApplyDeBuff()
{
	NodeName = TEXT("ApplyeDeBuff");
}

EBTNodeResult::Type UBTTask_ApplyDeBuff::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ControllingPawn->GetController()->StopMovement();

	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	EDEBUFFTYPE DEBUFFTYPE_var = EDEBUFFTYPE::WEAK;

	switch (debufftype)
	{
	case DEBUFFTYPE_WRAPPER::SHIELDDECREASE:
		DEBUFFTYPE_var = EDEBUFFTYPE::WEAK;
		break;
	case DEBUFFTYPE_WRAPPER::DAMAGEDECREASE:
		DEBUFFTYPE_var = EDEBUFFTYPE::DAMAGEDECREASE;
		break;
	case DEBUFFTYPE_WRAPPER::HEALINGDECREASE:
		DEBUFFTYPE_var = EDEBUFFTYPE::HEALINGDECREASE;
		break;
	case DEBUFFTYPE_WRAPPER::STUN:
		DEBUFFTYPE_var = EDEBUFFTYPE::STUN;
		break;
	case DEBUFFTYPE_WRAPPER::APPLYPIERCEDAMAGE:
		DEBUFFTYPE_var = EDEBUFFTYPE::APPLYPIERCEDAMAGE;
		break;
	default:
		EBTNodeResult::Failed;
		break;
	}

	ANonPlayerCharacter* MyNPC = Cast<ANonPlayerCharacter>(ControllingPawn);
	UObject* OTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
	AGCharacter* Target;
	if (OTarget)
	{
		Target = Cast<AGCharacter>(OTarget);
		if (MyNPC && Target)
		{
			Target->ApplyDeBuff(DEBUFFTYPE_var, amount, turnCount);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
