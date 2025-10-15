#include "GameLevelPalace.h"

#include "G/GGameInstance.h"
#include "G/Manager/InGameDataManager.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/Map/MapUI.h"
#include "G/UI/Map/MapWidget.h"
#include "G/UI/Map/WorldmapWidget.h"
#include "Kismet/GameplayStatics.h"

AGameLevelPalace::AGameLevelPalace()
{
}

void AGameLevelPalace::BeginPlay()
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

	GameInstance->GetInGameUI()->GetMinimap()->ChangeToPalace();
	GameInstance->GetGameMenuManager()->GetMapUI()->GetWorldmapWidget()->ChangeToPalace();
	
	if(GameInstance && GameInstance->GetInGameDataManager())
	{
		GameInstance->GetInGameDataManager()->ChangeCurrentLevel(EGameLevel::Palace);
	}
}
