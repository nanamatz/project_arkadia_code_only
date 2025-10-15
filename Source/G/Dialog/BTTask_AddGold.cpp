// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/BTTask_AddGold.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Manager/InventoryManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AddGold::UBTTask_AddGold()
{
	NodeName = TEXT("AddGold");
}

EBTNodeResult::Type UBTTask_AddGold::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 게임 인스턴스 저장
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GameInstance->inventory->AddGold(gold);

	return EBTNodeResult::Succeeded;
}
