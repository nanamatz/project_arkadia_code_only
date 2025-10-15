// Fill out your copyright notice in the Description page of Project Settings.


#include "G/BT/Service/BTS_MoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/NPCAIController.h"

UBTS_MoveTo::UBTS_MoveTo()
{
	NodeName = "BTS_MoveTo";
	bNotifyTick = true;
}

void UBTS_MoveTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 어서치 노드가 속한 블랙보드 컴포넌트 얻기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	// 에이전트의 AI 컨트롤러 얻기
	ANPCAIController* AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
	}


	/*// 현재 코스트
	float CurCost = BlackboardComp->GetValueAsFloat(ANPCAIController::CurCostKey);

	FVector HomePos = BlackboardComp->GetValueAsVector(ANPCAIController::HomePosKey);
	FVector TargetPos = BlackboardComp->GetValueAsVector(ANPCAIController::PatrolPosKey);
	FVector CurPos = AIController->GetPawn()->GetActorLocation();
	float MoveDis = FVector::Dist(HomePos, CurPos);

	// HomePos를 계속 갱신하면서 틱마다 이동거리를 계산해서 코스트 차감
	BlackboardComp->SetValueAsVector(ANPCAIController::HomePosKey, CurPos);

	// 코스트 계산
	CurCost -= MoveDis/100.0f;
	if (CurCost <= 0.0f) {
		// 코스트 모두 사용하면 해당 Task 종료
		OwnerComp.RequestExecution(EBTNodeResult::Failed);
	}
	BlackboardComp->SetValueAsFloat(ANPCAIController::MoveCostKey, CurCost);*/
}
