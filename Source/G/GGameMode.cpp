// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGameMode.h"
#include "GGameInstance.h"
#include "GPlayerCamera.h"
#include "GPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "TimerManager.h"
#include <G/Util/Define.h>

#include "Manager/DataManager.h"
#include "Manager/InGameDataManager.h"

void AGGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

AGGameMode::AGGameMode()
{
	CreateAndSetPawn();
}


// 임시
void AGGameMode::CreateAndSetPawn()
{
	// 컨트롤러 설정
	PlayerControllerClass = AGPlayerController::StaticClass();
	// 카메라 설정
	DefaultPawnClass = AGPlayerCamera::StaticClass();
}

void AGGameMode::CreateCharacter()
{
}

APlayerCharacter* AGGameMode::GetCharacter(int num)
{
	return GameInstance->GetInGameDataManager()->GetPlayerCharacter(static_cast<ECharacterName>(num));
}
