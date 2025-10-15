// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Level/GameLevel_Stage1.h"

#include "G/GGameInstance.h"
#include "G/GGameMode.h"
#include <Kismet/GameplayStatics.h>

#include "G/G.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/Map/MapUI.h"
#include "G/UI/Map/MapWidget.h"
#include "G/UI/Map/WorldmapWidget.h"
#include "G/Util/Define.h"

#include "LevelSequencePlayer.h"
#include "G/Manager/TutorialManager.h"

AGameLevel_Stage1::AGameLevel_Stage1()
{
}

void AGameLevel_Stage1::BeginPlay()
{
	Super::BeginPlay();
	
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	/*if(GameInstance->GetIsStart())
		GameInstance->LoadStreamingLevel(GetWorld(), FName(MapPath + "Stage1/Beach"));	
	else
		GameInstance->LoadStreamingLevel(GetWorld(), FName(MapPath + "Stage1/Desert4"));*/
	
	GameInstance->LevelInit();
	
	for (auto& characterInfo : GameInstance->GetInGameDataManager()->GetSpawnPlayerCharacterInfoMap())
	{
		FVector SpawnLocation = characterInfo.Value.CharacterLocation;

		APlayerCharacter* spawnCharacter = GameInstance->PlayerCharacterSpawnAndGet(characterInfo.Key, SpawnLocation);

		if(spawnCharacter)
			GameInstance->Update_UI(spawnCharacter);
	}

	
	GameInstance->bIsStart = false;
	
	GameInstance->GetInGameUI()->GetMinimap()->ChangeToStage1();
	GameInstance->GetGameMenuManager()->GetMapUI()->GetWorldmapWidget()->ChangeToStage1();
}
