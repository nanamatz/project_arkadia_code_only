// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CharacterAIController.generated.h"

class UGGameInstance;

/**
 * 
 * �÷��̾� ĳ���� AI Controller
 * ��������� ��, �ƴ� �� �ൿ�� �޶���
 * ��������� �� - �ش� ���� ĳ���͸� �̵�
 * �ƴ� �� - ��� ĳ���Ͱ� �������� �̵�
 * 
 */
UCLASS()
class G_API ACharacterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGGameInstance* GameInstance;

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
	FVector _destination;

	void OnRepeatTimer();

public:
	ACharacterAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	// ������ �̵�
	UFUNCTION()
	void MoveToDestination();
	void CombatMoveToDestination();
};
