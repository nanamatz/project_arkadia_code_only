// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ApplyBuff.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "G/PlayerCharacter.h"
#include "G/Player/CharacterInfoStruct.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTTask_ApplyBuff::UBTTask_ApplyBuff()
{
	NodeName = TEXT("ApplyeBuff");
}

EBTNodeResult::Type UBTTask_ApplyBuff::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	EBUFFTYPE bufftype_var = EBUFFTYPE::SHIELDINCREASE;

	switch (bufftype)
	{
	case BUFFTYPE_WRAPPER::SHIELDINCREASE:
		bufftype_var = EBUFFTYPE::SHIELDINCREASE;
		break;
	case BUFFTYPE_WRAPPER::INSIGHTINCREASE:
		bufftype_var = EBUFFTYPE::INSIGHTINCREASE;
		break;
	case BUFFTYPE_WRAPPER::COSTINCREASE:
		bufftype_var = EBUFFTYPE::COSTINCREASE;
		break;
	case BUFFTYPE_WRAPPER::STRENGTHINCREASE:
		bufftype_var = EBUFFTYPE::STRENGTHINCREASE;
		break;
	case BUFFTYPE_WRAPPER::LUCKYINCREASE:
		bufftype_var = EBUFFTYPE::LUCKYINCREASE;
		break;
	case BUFFTYPE_WRAPPER::MOVECOSTINCREASE:
		bufftype_var = EBUFFTYPE::MOVECOSTINCREASE;
		break;
	default:
		EBTNodeResult::Failed;
		break;
	}

	ANonPlayerCharacter* MyNPC = Cast<ANonPlayerCharacter>(ControllingPawn);
	if (MyNPC)
	{
		MyNPC->ApplyBuff(bufftype_var, amount, turnCount);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
