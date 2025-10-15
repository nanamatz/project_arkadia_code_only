// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatArea.h"
#include "NPCAIController.generated.h"

class APlayerCharacter;

/**
 * NPC 캐릭터 AI Controller
 */
UCLASS()
class G_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere)
	class UBlackboardData* BBAsset;

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	void OnRepeatTimer();

public:
	static const FName SelfActor; // 현재 위치
	static const FName HomePosKey; // 현재 위치
	static const FName PatrolPosKey; // 랜덤 이동 위치
	static const FName TargetPosKey; // 타겟 이동 위치
	static const FName BControllingKey; // 컨트롤하는지 확인
	static const FName TargetKey; // 타겟 오브젝트
	static const FName CombatAreaKey; // 현재 전투지역 오브젝트
	static const FName CanTargetAttackKey; // 현재 전투지역 오브젝트
	static const FName MoveCostKey; // 현재 이동 코스트
	static const FName CurCostKey; // 현재 남은 코스트
	static const FName TargetDistance; // 타겟과의 거리
	static const FName BCombatKey;
	static const FName OriginPosKey; // 처음 생성 위치

	static const FName BSelectSkillKey;
	static const FName BUseSkillKey;


	ANPCAIController();
	// AIController가 AI캐릭터를 소유할 때 호출
	virtual void OnPossess(APawn* InPawn) override;
	// AIController가 캐릭터를 더이상 소유하지 않을 때 호출
	virtual void OnUnPossess() override;

	void RunAI();
	void StopAI();

	void SetBControlling(bool bValue);
	void SetMoveCostKey(float cost);
	void SetCurCostKey(int cost);
	void SetCombatArea(class ACombatArea* CA);
	void SetBCombat(bool bValue);
	void SetOriginLocation(const FVector& location);
	
	void AddMoveCost(float cost);
};
