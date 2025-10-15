// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "G/Player/CharacterInfoStruct.h"
#include "GameFramework/Character.h"
#include "GCharacter.generated.h"


UCLASS(Blueprintable)
class AGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCharacter();

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* NavCollision;

	UPROPERTY(VisibleAnywhere, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
	class UCharacterInfoComponent* CharacterInfoComponent;

	UPROPERTY()
	class UGGameInstance* GameInstance;

	// 스킬시스템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkillSystemComponent* SkillSystemComponent;

	
	FTimerHandle _rotateHandle;

public:
	virtual void BeginPlay() override;

	void InitCharacter();
	virtual void BeginDestroy() override;

	// 캐릭터 번호 가져오기
	int GetCharacterNum() { return 0; }
	class USkillSystemComponent* GetSkillSystemComponent() { return SkillSystemComponent; }
	UCharacterInfoComponent* GetCharacterInfoManager() { return CharacterInfoComponent; }
	struct FCharacterInfo& GetCharacterInfo();
	int32 GetCurrentHP();
	int32 GetMaxHP();
	int32& GetCurrentHPRef();
	int32& GetMaxHPRef();
	int32 GetCurrentCost();
	int32 GetMaxCost();
	int32 GetCurrentShield();
	int32 GetMinDamage();
	int32 GetMaxDamage();
	bool GetBDeath();
	FText GetCharacterName();
	UTexture2D& GetPortrait();
	enum ECharacterName GetECharacterName();

	void FillCost();

	// 데미지 받기
	virtual int32 TakeAttackDamage(int32 _attackDamage, bool bPiercingDamage = false);
	virtual void Respawn();
	virtual void Death();

	int AddCurrentCost(int _cost);
	int AddCurrentHP(float _hp);

	/*
		
				이동 처리
		
		*/
	void SetMoveCost(float _num);
	float GetMoveCost();
	void AddMoveCost(int value);
	void UseMoveCost(float value);


	/*	버프
	 *	_buffNum : 버프 종류, BUFFTYPE::버프 종류로 선언
	 *	_amount : 버프량
	 *	_turnCount : 지속시간
	 */
	void ApplyBuff(EBUFFTYPE _buffNum, float _amount, int32 _turnCount);
	bool CheckBuff(EBUFFTYPE type);
	/* 디버프
	 *	_deBuffNum : 디버프 종류, BUFFTYPE::디버프 종류로 선언
	 *	_amount : 디버프량
	 *	_turnCount : 지속시간
	 */
	void ApplyDeBuff(EDEBUFFTYPE _deBuffNum, float _amount, int32 _turnCount);
	bool CheckDebuff(EDEBUFFTYPE type);

	void CountBuffAndDeBuff();
	void CountSkillCoolDown();

	void CharacterCombatTurnStart();
	void CharacterCombatTurnEnd();

	//! @breif 전투모드일 때, Navgation처리 
	//! @param b : True면 경로 계산 시 장애물 처리, False는 자신이 이동할 때 장애물 처리 해제
	void SetAffectNavigation(bool b);

	// 타겟 방향으로 회전
	void RotateTowardTarget(AGCharacter* Target);

	// 마우스 방향으로 회전
	void RotateTowardMousePos(FVector MousePos);

	void ActiveHighlight();
	void DeActiveHighlight();
	void ActiveHighlightInMoment(float time);
};
