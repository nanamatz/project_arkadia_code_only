// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Level/GameLevelJail.h"

#include "G/GGameInstance.h"
#include "G/GGameMode.h"
#include <Kismet/GameplayStatics.h>

#include "G/G.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/Inventory/InventoryUI.h"
#include "G/UI/Map/MapUI.h"
#include "G/UI/Map/MapWidget.h"
#include "G/UI/Map/WorldmapWidget.h"
AGameLevelJail::AGameLevelJail()
{

}

void AGameLevelJail::BeginPlay()
{
	Super::BeginPlay();

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GameInstance->LevelInit();
	
	for (auto& characterInfo : GameInstance->GetInGameDataManager()->GetSpawnPlayerCharacterInfoMap())
	{
		FVector SpawnLocation = characterInfo.Value.CharacterLocation;
		
		APlayerCharacter* spawnCharacter = GameInstance->PlayerCharacterSpawnAndGet(characterInfo.Key, SpawnLocation);

		if(spawnCharacter)
			GameInstance->Update_UI(spawnCharacter);
	}
	
	GameInstance->GetInGameUI()->GetMinimap()->ChangeToCaveB1();
	GameInstance->GetGameMenuManager()->GetMapUI()->GetWorldmapWidget()->ChangeToCaveB1();
	GameInstance->GetGameMenuManager()->GetMapUI()->SetZoom(0.17f);

	if(GameInstance && GameInstance->GetInGameDataManager())
	{
		GameInstance->GetInGameDataManager()->ChangeCurrentLevel(EGameLevel::CaveJail);
	}
}
