// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAIController.h"

#include "GGameMode.h"
#include "NavigationSystem.h"
#include "Level/GameLevel.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <Kismet/GameplayStatics.h>

#include "GGameInstance.h"
#include "GPlayerCamera.h"
#include "Controller/PlayerMoveController.h"
#include "Manager/CombatManager.h"

ACharacterAIController::ACharacterAIController()
{
	RepeatInterval = 0.1f;
}


void ACharacterAIController::OnPossess(APawn* InPawn)
{
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Super::OnPossess(InPawn);
	// Ÿ�̸� ȣ��
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ACharacterAIController::OnRepeatTimer,
	                                       RepeatInterval, true);
}

void ACharacterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ACharacterAIController::OnRepeatTimer()
{
	if (!GameInstance->GetCombatManager()->CheckCombatTure())
	{
		APlayerCharacter* controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
		if (!controlCharacter)
		{
			return;
		}
		APlayerCharacter* myCharacter = Cast<APlayerCharacter>(GetPawn());

		if (!myCharacter || myCharacter->GetBDeath())
		{
			return;
		}

		FVector destination = controlCharacter->GetActorLocation();
		FVector direction = controlCharacter->GetActorForwardVector();

		if (myCharacter == controlCharacter)
		{
			_destination = GameInstance->GetPlayerMoveController()->GetDestination();
			MoveToDestination();
			return;
		}


		// 이동 시 캐릭터 위치를 삼각형 형태로 만들기
		int count = 0;
		for (APlayerCharacter* character : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
		{
			if (myCharacter != controlCharacter)
			{
				count++;
			}

			if (character == myCharacter)
			{
				break;
			}
		}

		float pivotDis = 150.0f;
		float angle[4] = {0, 30, -30, 60};
		FVector axis(0, 0, 1);
		direction = direction.RotateAngleAxis(angle[count], axis);
		_destination = destination - (pivotDis * direction);
		MoveToDestination();
	}
}

void ACharacterAIController::MoveToDestination()
{
	// UE_LOG(LogTemp, Log, TEXT("NOTCOMBAT MOVE : %s"), *_destination.ToString());
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, _destination);
}

void ACharacterAIController::CombatMoveToDestination()
{
	AGGameMode* gameMode = Cast<AGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	APlayerCharacter* playerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();

	if (playerCharacter->GetCharacterNum() == Cast<AGCharacter>(GetPawn())->GetCharacterNum())
	{
		// �̵��ϸ鼭 �ൿ�� ����, �ൿ���� ��� �Ҹ��ϸ� ���߱�
		FVector destination = GameInstance->GetPlayerMoveController()->GetDestination();
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, destination);
	}
}
