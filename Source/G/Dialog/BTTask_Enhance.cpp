// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/BTTask_Enhance.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/UI/GameMenuManager.h"

UBTTask_Enhance::UBTTask_Enhance()
{
	NodeName = TEXT("CallEnhanceUI");
}

EBTNodeResult::Type UBTTask_Enhance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 게임 인스턴스 저장
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->GetGameMenuManager()->OnEnhenceUI();

	return EBTNodeResult::Succeeded;
}
