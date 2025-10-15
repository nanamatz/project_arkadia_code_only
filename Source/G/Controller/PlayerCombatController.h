// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/GCharacter.h"
#include "G/PlayerCharacter.h"
#include "G/Controller/ControllerBase.h"
#include "G/Skill/SkillBase.h"
#include "G/Util/Define.h"
#include "PlayerCombatController.generated.h"

/**
 *
 *	전투 상황 시 플레이어 입력 관리
 * 
 */
UCLASS()
class G_API UPlayerCombatController : public UControllerBase
{
	GENERATED_BODY()

	EActionMode _playerInputState;

	UPROPERTY()
	class AGCharacter* _selectedSkillTarget;

	UPROPERTY()
	class ASkillRangeActor* SkillRangeActor;

public:
	EActionMode GetPlayerInputState()
	{
		return _playerInputState;
	}

	// cost : cost를 사용하는 액션인 경우 입력
	void SetPlayerInputState(EActionMode actionState, int cost = 0);

	// 전투 중 입력 상태 확인
	bool CheckPlayerInputState(EActionMode actionState);

	void SkillPreProcessing(int skillNum, class USkillButton* target_button);

	void SetStateActionNone();
	void SetStateBeforeSkillAction();
	void SetStateClickSkill();
	void SetStateClickTarget();
	void SetStateActionSkill(int cost);
	class AGCharacter* GetTargetUnderCursor(ESkillTargetType TargetType);
	void SetStateMoving(int cost);
	void UseCost(int cost);

	void HighlightTarget();
	void HighlightSingleTarget();
	void HighlightMultiTarget();


	/*// 좌클릭 공격 액션
	void OnClickUseSkill();

	// 액션 취소
	void OnClickCancelAction();
	
	// 커서 아래 위치 찾기
	FVector GetLocationUnderCursor();

	// 경로 표시
	void DrawPath();

	// 사용 Cost 계산식
	int CalRequirementCost(float _distance) { return _distance / 10.0f + 1; }
	
	// 거리 텍스트 표시
	void UpdateDistanceText(float Distance);*/
};
