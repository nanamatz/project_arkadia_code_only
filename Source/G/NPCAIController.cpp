// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "NavigationSystem.h"
#include "Level/GameLevel.h"
#include "PlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

#include "G.h"
#include "NonPlayerCharacter.h"

const FName ANPCAIController::SelfActor(TEXT("SelfActor"));
const FName ANPCAIController::HomePosKey(TEXT("HomePos"));
const FName ANPCAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ANPCAIController::TargetPosKey(TEXT("TargetPos"));
const FName ANPCAIController::TargetKey(TEXT("Target"));
const FName ANPCAIController::BControllingKey(TEXT("BControlling"));
const FName ANPCAIController::CombatAreaKey(TEXT("CombatArea"));
const FName ANPCAIController::CanTargetAttackKey(TEXT("CanTargetAttack"));
const FName ANPCAIController::MoveCostKey(TEXT("MoveCost"));
const FName ANPCAIController::CurCostKey(TEXT("CurCost"));
const FName ANPCAIController::TargetDistance(TEXT("TargetDistance"));
const FName ANPCAIController::BCombatKey(TEXT("BCombat"));
const FName ANPCAIController::OriginPosKey(TEXT("OriginPos"));
const FName ANPCAIController::BSelectSkillKey(TEXT("BSelectSkill"));
const FName ANPCAIController::BUseSkillKey(TEXT("BUseSkill"));

ANPCAIController::ANPCAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/TopDownCPP/Blueprints/BB_NPC.BB_NPC'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/TopDownCPP/Blueprints/BT_NPC.BT_NPC'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	RepeatInterval = 0.1f;
}

// AIController가 AI캐릭터를 소유할 때 호출
void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		// 액터의 위치정보를 HomePosKey에 저장
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		ANonPlayerCharacter* NPC = Cast<ANonPlayerCharacter>(InPawn);
	}

	RunAI();


	/*GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ANPCAIController::OnRepeatTimer, RepeatInterval, true);*/
}

// AIController가 캐릭터를 더이상 소유하지 않을 때 호출
void ANPCAIController::OnUnPossess()
{
	Super::OnUnPossess();
	StopAI();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ANPCAIController::OnRepeatTimer()
{
}

void ANPCAIController::RunAI()
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		RunBehaviorTree(BTAsset);
	}
}

void ANPCAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BehaviorTreeComponent)
	{
		return;
	}

	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void ANPCAIController::SetBControlling(bool bValue)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		// 액터의 위치정보를 HomePosKey에 저장
		Blackboard->SetValueAsBool(BControllingKey, bValue);
	}
}

void ANPCAIController::SetMoveCostKey(float cost)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		Blackboard->SetValueAsFloat(MoveCostKey, cost);
	}
}

void ANPCAIController::SetCurCostKey(int cost)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		Blackboard->SetValueAsInt(CurCostKey, cost);
	}
}

void ANPCAIController::SetCombatArea(ACombatArea* CA)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (CA && UseBlackboard(BBAsset, bb_comp))
	{
		//LOG(Log, "%s", *CA->GetName());
		LOG(Log, "BlackBoard Set CombatArea");
		Blackboard->SetValueAsObject(CombatAreaKey, CA);
	}else{
		LOG(Log, "Fail BlackBoard Set CombatArea");
	}
}

void ANPCAIController::SetBCombat(bool bValue)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		if (bValue)
		{
			StopMovement();
		}
		Blackboard->SetValueAsBool(BCombatKey, bValue);
	}
}

void ANPCAIController::SetOriginLocation(const FVector& location)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		Blackboard->SetValueAsVector(OriginPosKey, location);
	}
}

void ANPCAIController::AddMoveCost(float cost)
{
	UBlackboardComponent* bb_comp = Blackboard.Get();
	if (UseBlackboard(BBAsset, bb_comp))
	{
		float curMoveCost = Blackboard->GetValueAsFloat(MoveCostKey);
		SetMoveCostKey(curMoveCost + cost);
		/*UE_LOG(LogTemp, Warning, TEXT("CurCost : %.2f"), curCost);
		UE_LOG(LogTemp, Warning, TEXT("cost : %.2f"), cost);*/
	}
}
