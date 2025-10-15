// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/BTTask_AddExp.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Manager/InventoryManager.h"
#include "G/Manager/InGameDataManager.h"
#include "G/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AddExp::UBTTask_AddExp()
{
	NodeName = TEXT("AddExp");
}

EBTNodeResult::Type UBTTask_AddExp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 게임 인스턴스 저장
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	for (auto iter : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
	{
		iter->GetCharacterInfoManager()->AddCurrentExp(Exp);
	}
	
	return EBTNodeResult::Succeeded;
}
