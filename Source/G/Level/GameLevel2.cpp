// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Level/GameLevel2.h"

#include "G/GGameInstance.h"
#include "G/GGameMode.h"

#include <Kismet/GameplayStatics.h>

AGameLevel2::AGameLevel2()
{
}

void AGameLevel2::BeginPlay()
{
	Super::BeginPlay();
	//FVector SpawnLocation = FVector(-1890.000000,-8810.000000, 2208.000000);
	//FVector SpawnLocation = FVector(27499.521484,927.709229,191.481262);
	//FVector SpawnLocation = FVector(-680.000000, -8140.000000, 2270.000000);
	//FVector SpawnLocation = FVector(0, 0, 1770.0f);
	//FVector SpawnLocation = FVector(-265.0f, -211.0f, 300.0f); // Test Map Start Pos
	//FVector SpawnLocation = FVector(1730.0f, 3520.0f, 1280.0f); // Jail Map Start Pos
	//FVector SpawnLocation = FVector(-31299.0f, 97942.0f, 403.0f); // Normal Map Start Pos
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->LevelInit();

	for (auto& characterInfo : GameInstance->GetInGameDataManager()->GetSpawnPlayerCharacterInfoMap())
	{
		UE_LOG(LogTemp, Log, TEXT("GameLevel Begin Play, Get Player Character"));
		FVector SpawnLocation = FVector(-265.0f, -211.0f, 300.0f); // Test Map Start Pos
		if (GameInstance->bIsStart)
		{
			SpawnLocation = FVector(-31299.0f, 97942.0f, 403.0f);
		}
		else
		{
			SpawnLocation = FVector(-4690.000000, -11670.000000, 1540.000000);
		}
		APlayerCharacter* spawnCharacter = GameInstance->PlayerCharacterSpawnAndGet(characterInfo.Key, SpawnLocation);

	}
}
