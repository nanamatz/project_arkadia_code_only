// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevel.h"

#include "G/GGameInstance.h"
#include "G/GGameMode.h"
#include <Kismet/GameplayStatics.h>

#include "G/Manager/CombatManager.h"

AGameLevel::AGameLevel()
{
}

void AGameLevel::BeginPlay()
{
	Super::BeginPlay();
	// ĳ���� ����
	/*UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->LevelInit();*/

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->GetCombatManager()->Init(GameInstance);

	if (GameInstance->bIsStart == false)
	{
		GameInstance->GetInGameDataManager()->Init(GameInstance);
		FVector SpawnLocation = FVector(25000.0f, 2700.0f, 500.0f); // Normal Map Start Pos
		GameInstance->PlayerCharacterSpawnAndGet(ECharacterName::Cardenio, SpawnLocation);
	}
	else
	{
		for (auto& characterInfo : GameInstance->GetInGameDataManager()->GetSpawnPlayerCharacterInfoMap())
		{
			UE_LOG(LogTemp, Log, TEXT("GameLevel Begin Play, Get Player Character"));
			FVector SpawnLocation = FVector(25000.0f, 2700.0f, 500.0f); // Normal Map Start Pos
			APlayerCharacter* spawnCharacter = GameInstance->PlayerCharacterSpawnAndGet(
				characterInfo.Key, SpawnLocation);
		}
	}
	GameInstance->LevelInit();
}
