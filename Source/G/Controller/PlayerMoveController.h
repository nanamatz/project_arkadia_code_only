// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Controller/ControllerBase.h"
#include "PlayerMoveController.generated.h"

/**
 * 
 */
UCLASS()
class G_API UPlayerMoveController : public UControllerBase
{
	GENERATED_BODY()
	// 이동 목적지
	FVector _destination;

	bool _isPlayerCanMove;

public:
	FVector GetDestination() { return _destination; }

	// true : 이동 가능, false : 이동 불가능
	bool IsPlayerCanMove() { return _isPlayerCanMove; }

	void SetDestination(FVector& Location)
	{
		// UE_LOG(LogTemp, Warning, TEXT("set destinantion : %s"), *Location.ToString());
		_destination = Location;
	}

	void SetIsPlayerCanMove(bool bValue) { _isPlayerCanMove = bValue; }

	/*// 경로 표시
	void DrawPath();
	// m 거리 계산
	float ChangeDistanceNotation(float _dis);
	// start, end m 거리 계산
	float CalDistance(FVector& Start, FVector& End);
	// nav 거리 계산
	bool GetMousePointNavDistance(float& _out);
	// 사용 Cost 계산식
	int CalRequirementCost(float _distance) { return _distance / 10.0f + 1; }
	// 거리 텍스트 표시
	void UpdateDistanceText(float Distance);*/
};
