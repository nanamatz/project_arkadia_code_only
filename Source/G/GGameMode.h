// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/GameModeBase.h"

#include <G/Util/Define.h>

#include "Manager/InGameDataManager.h"
#include "GGameMode.generated.h"

/*
* 
* 인스턴스에 저장된 플레이어 캐릭터 정보를 가지고 생성
* 맵에 생성된 플레이어 캐릭터에 대한 정보를 관리
* 플레이어 캐릭터 이동 지점 관리
* 
* 전투모드 관리
* 전투 순서를 큐를 통해 관리
* 
*/

UENUM()
enum ESkill
{
	SKILLPA,
	SKILL1,
	SKILL2,
	SKILL3,
	SKILL4,
	SKILL5,
	SKILL6,
	SKILL7,
	SKILL8,
	SKILL9,
	SKILLNONE
};

UCLASS(minimalapi)
class AGGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UGGameInstance* GameInstance;

	virtual void BeginPlay() override;

	FTimerHandle MyDelayTimerHandle;

public:
	AGGameMode();

	UFUNCTION()
	void CreateAndSetPawn();

	void CreateCharacter();

	// 저장된 캐릭터를 가져가기
	UFUNCTION()
	APlayerCharacter* GetCharacter(int num);
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetControlCharacter()
	{
		return GameInstance->GetInGameDataManager()->GetControlCharacter();
	};
};
