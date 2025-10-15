// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Level/TestLevel.h"

#include "G/GGameInstance.h"
#include "G/GGameMode.h"
#include <Kismet/GameplayStatics.h>

#include "G/G.h"

void ATestLevel::BeginPlay()
{
	Super::BeginPlay();
    
	LOG(Log, "AGameLevel_Stage1 BeginPlay");
    GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GameInstance->bIsStart = true;
    GameInstance->LevelInit();
    
    for (auto& characterInfo : GameInstance->GetInGameDataManager()->GetSpawnPlayerCharacterInfoMap())
    {
        UE_LOG(LogTemp, Log, TEXT("GameLevelStage1 Begin Play, Get Player Character"));
        FVector SpawnLocation = FVector(-265.0f, -211.0f, 300.0f); // Test Map Start Pos

        APlayerCharacter* spawnCharacter = GameInstance->PlayerCharacterSpawnAndGet(characterInfo.Key, SpawnLocation);
        LOG(Log, "%f %f", SpawnLocation.X, SpawnLocation.Y)
        GameInstance->Update_UI(spawnCharacter);
    }
	GameInstance->bIsStart = false;
}
