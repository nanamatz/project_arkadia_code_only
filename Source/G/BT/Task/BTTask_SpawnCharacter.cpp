// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpawnCharacter.h"
#include "G/NonPlayerCharacter.h"
#include "G/NPCAIController.h"
#include "G/CombatArea.h"
#include "G/GGameInstance.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Manager/CombatManager.h"
#include "G/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "G/Item/ItemBase.h"

UBTTask_SpawnCharacter::UBTTask_SpawnCharacter()
{
	NodeName = TEXT("SpawnCharacter");
}

EBTNodeResult::Type UBTTask_SpawnCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UCombatManager* combatmanager = GameInstance->GetCombatManager();

	//�� ������Ű��
	ANonPlayerCharacter* MyNPC = Cast<ANonPlayerCharacter>(ControllingPawn);
	if (MyNPC == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	ACombatArea* combatarea = MyNPC->GetCombatArea();

	TArray<class ANonPlayerCharacter*> enemylist = combatarea->GetEnemyList();

	FVector SpawnLocation = MyNPC->GetActorLocation();
	float randomx = FMath::RandRange(50.0f, 100.0f);
	float randomy = FMath::RandRange(50.0f, 100.0f);
	SpawnLocation.X += randomx;
	SpawnLocation.Y += randomy;
	SpawnLocation.Z += randomy;

	ANonPlayerCharacter* SpawnNPC = SpawnNPC = MyNPC->GetWorld()->SpawnActor<ANonPlayerCharacter>(
		spawnTarget.Get(), SpawnLocation, FRotator::ZeroRotator);

	SpawnNPC->SetCombatArea(combatarea);
	SpawnNPC->SetSpawnNPC(*SpawnNPC->GetClass()->GetName());

	SpawnNPC->SetRewardGold(0);

	// ������ ����
	TArray<struct FRewardItemSetting> Items;
	Items.Empty();
	SpawnNPC->SetRewardItem(Items);

	/*
	if (SpawnNPC->GetBDeath())
	{
		SpawnNPC->SetActorLocation(SpawnLocation);

		SpawnNPC->SetSpawnNPC(EnemyNumber, *SpawnNPC->GetClass()->GetName());

		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
	*/

	//���ƺ��� �ϱ�
	UPROPERTY
	()
	TArray<APlayerCharacter*> playerCharacters = GameInstance->GetInGameDataManager()->GetPlayerCharacterList();
	if (playerCharacters.IsValidIndex(0))
	{
		SpawnNPC->RotateTowardTarget(playerCharacters[0]);
	}

	//CombatManager ����
	combatmanager->AddNPCSpawn(SpawnNPC);

	return EBTNodeResult::Succeeded;
}
