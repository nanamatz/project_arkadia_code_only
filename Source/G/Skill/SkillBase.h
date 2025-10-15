// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/CharacterInfoComponent.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class G_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	USkillBase();

protected:
	UPROPERTY()
	class USkillSlot* mySkillSlot;

	const ECollisionChannel ForwardSkillChannel = ECC_GameTraceChannel4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkillInfoManager* skillInfoManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillInfoManager> BP_SkillInfo;

	UPROPERTY()
	class AGCharacter* _myCharacter;

	UPROPERTY()
	FTimerHandle skillTimerHandle;

	UPROPERTY()
	FTimerHandle unusedHandle;

	UPROPERTY()
	int32 totalDamage;

	UPROPERTY(EditAnywhere)
	bool bHasAnim = true;

public:
	/* Skill Info Manager Function */

	void InitSkill();

	class AGCharacter* GetMyCharacter(){return _myCharacter;}
	struct FSkillStruct& GetSkillInfo();
	class UTexture2D* GetSkillThumnail();

	UFUNCTION()
	class USkillBase* GetMySkillClass();
	float GetCharacterRange();
	int GetCost();
	class USkillInfoManager* GetSkillInfoManager() { return skillInfoManager; }
	int32 GetTotalDamage() const { return totalDamage; }
	float GetSkillCriticalRate();

	void SetPlayerCharacter(class AGCharacter* _playerCharacter);
	// 처음 스킬 생성 세팅
	void SetSkill(class AGCharacter* _playerCharacter, struct FSkillStruct* _skillInfo);
	void SetSkill(AGCharacter* _playerCharacter);
	void SetThumnail(UTexture2D* Thumnail);

	UFUNCTION()
	virtual bool CheckSelectPossibility();

	// 사거리 타입에 따라 사용가능 여부 확인
	UFUNCTION()
	virtual bool CheckSkillPossibility();

	// 타겟이 사거리 안에 있는지 확인
	UFUNCTION()
	bool CheckTargetInAttackRange(AGCharacter* Target);

	UFUNCTION()
	bool CheckForwardSkillPossibility();

	/*
	
			스킬 처리
	
	*/
	UFUNCTION()
	bool CheckCurCoolDown();
	int32 CountCurCoolDown();
	// 스킬 시작
	UFUNCTION()
	virtual void SkillStart();
	UFUNCTION()
	void PlaySkillAnimation();

	// 스킬 사용
	UFUNCTION()
	virtual void UseSkill();

	// 스킬 사용 끝
	virtual void SkillEnd();
	
	int32 TargetAttack(class AGCharacter* target, struct FSkillCastAttack* _attack = nullptr);
	bool CheckHit(class AGCharacter* target);
	void TargetBuff(class AGCharacter* target);
	void TargetDebuff(class AGCharacter* target);
	void GetOverlapObjectAttack(float radius);
	TArray<class AGCharacter*> GetSkillPosOverlapCharacters(float radius = 0);

	// 광역 데미지 처리
	int GetOverlapObjectAttack();
	
};
