// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ColleaguesRecruitment.h"
#include "GGameMode.h"
#include "NPCAIController.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "Skill/SkillSystemComponent/SkillSystemComponent.h"

UBTTask_ColleaguesRecruitment::UBTTask_ColleaguesRecruitment()
{
	NodeName = TEXT("ColleaguesRecruitment");
	GameMode = Cast<AGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

EBTNodeResult::Type UBTTask_ColleaguesRecruitment::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	RecruitmentNPC(NPC_num);

	//auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	//if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	//
	//FVector actorlocation = OwnerComp.GetOwner()->GetActorLocation();
	//actorlocation.Y -= 500.0f;
	//OwnerComp.GetOwner()->SetActorLocation(actorlocation);
	return EBTNodeResult::Succeeded;
}

void UBTTask_ColleaguesRecruitment::RecruitmentNPC(int n)
{
	if (n == 0)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		FVector spawnLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
		GameInstance->GetInGameDataManager()->AddSpawnPlayerCharacterInfoMap(ECharacterName::Fernando,
																   *GameInstance->GetDataManager()->GetPlayerCharacterInfoData(
																	   ECharacterName::Fernando));
		APlayerCharacter* newCharacter = GameInstance->PlayerCharacterSpawnAndGet(ECharacterName::Fernando, spawnLocation, FRotator::ZeroRotator, true);
		
		// UI 업데이트
		GameInstance->Update_UI(newCharacter);
	}
	else if (n == 1)
	{
		//Samira
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		FVector spawnLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
		GameInstance->GetInGameDataManager()->AddSpawnPlayerCharacterInfoMap(ECharacterName::Samira,
																   *GameInstance->GetDataManager()->GetPlayerCharacterInfoData(
																	   ECharacterName::Samira));
		APlayerCharacter* newCharacter = GameInstance->PlayerCharacterSpawnAndGet(ECharacterName::Samira, spawnLocation, FRotator::ZeroRotator, true);

		// UI 업데이트
		GameInstance->Update_UI(newCharacter);
	}
	else if (n == 2)
	{
		//RuthCinda
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		FVector spawnLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();

		GameInstance->GetInGameDataManager()->AddSpawnPlayerCharacterInfoMap(ECharacterName::RuthCinda,
															   *GameInstance->GetDataManager()->GetPlayerCharacterInfoData(
																   ECharacterName::RuthCinda));
		APlayerCharacter* newCharacter = GameInstance->PlayerCharacterSpawnAndGet(ECharacterName::RuthCinda, spawnLocation, FRotator::ZeroRotator, true);

		// UI 업데이트
		GameInstance->Update_UI(newCharacter);
	}
}
