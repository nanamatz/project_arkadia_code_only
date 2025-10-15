// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <G/Util/Define.h>
#include <G/Skill/SkillData/SkillStruct.h>
#include <G/Skill/SkillBase.h>

#include "SkillData/SkillAction/ActionBase.h"
#include "SkillData/SkillTreeNode/UpgradeNodeStruct.h"
#include "SkillInfoManager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class G_API USkillInfoManager : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	struct FSkillStruct skillInfo;

	UPROPERTY()
	int curCoolDown = 0;

public:
	void SetSkillInfo(FSkillStruct _skillInfo);

	FSkillStruct& GetSkillInfo() { return skillInfo; }
	USkillBase* GetSkillClass() { return skillInfo.skillClass.GetDefaultObject(); }
	class UAnimMontage* GetSkillAnimMontage() { return skillInfo.skillAnimMontage; }
	FText GetSkillName() { return skillInfo.Name; }
	FText GetDescription() { return skillInfo.Description; }
	UTexture2D* GetThumbnail() { return skillInfo.Thumbnail; }
	int GetCoolDown() { return skillInfo.CoolDown; }
	float GetCost() { return skillInfo.Cost; }
	// 스킬 범위 타입(캐릭터 중심 원, 마우스 중심 원, 캐릭터 중심 부채꼴
	ESkillRangeType GetSkillRangeType() { return skillInfo.SkillRangeType; }
	float GetCharacterRange() { return skillInfo.CharacterRange; }
	float GetMouseRange() { return skillInfo.MouseRange; }
	ESkillTargetType GetTargetType() { return skillInfo.TargetType; }
	ESkillType GetSkillType() {return skillInfo.SkillType;}
	int GetCurCoolDown() { return curCoolDown; }
	float GetSkillCastTime() { return skillInfo.SkillTiming; }
	TArray<struct FSkillCastAttack>& GetAttackList() { return skillInfo.AttackList; }
	TArray<struct FSkillCastBuff>& GetBuffList() { return skillInfo.BuffList; }
	TArray<struct FSkillCastDebuff>& GetDebuffList() { return skillInfo.DebuffList; }
	TArray<class UActionBase*> GetActionList() { return skillInfo.ActionList; }
	float GetSkillCriticalRate() { return skillInfo.AttackCriticalRate;}


	void SetSkillAnimMontage(class UAnimMontage* newAnimMontage) { skillInfo.skillAnimMontage = newAnimMontage; }
	void SetSkillRangeType(ESkillRangeType _type);
	void SetCharacterRange(float _range) { skillInfo.CharacterRange = _range; }
	void SetMouseRange(float _range) { skillInfo.MouseRange = _range; }
	void SetTargetType(ESkillTargetType type);
	void SetSkillType(ESkillType type) {skillInfo.SkillType = type;}
	void SetAttackType(EAttackType type);
	void SetCurCoolDown() { curCoolDown = skillInfo.CoolDown; }
	void SetCurCoolDown(int _num) { curCoolDown = _num; }
	void SetThumnail(UTexture2D* Thumnail) { skillInfo.Thumbnail = Thumnail; }

	void AddCost(int _addNum) { skillInfo.Cost += _addNum; }
	void AddCharacterRange(float _addNum);
	void AddMouseRange(float _addNum);
	void AddCurCoolDown(int _num);
	void AddCoolDown(int _num);
	void AddAttackDamage(int32 value);
	void AddDuration(int32 value);
	void AddAttackType(struct FSkillCastAttack newAttack);
	void AddBuffType(struct FSkillCastBuff newBuff);
	void AddDebuffType(struct FSkillCastDebuff newDebuff);
	void AddActionType(TSubclassOf<class UActionBase> newAction);
	void AddDescription(FText newDescription);
	void AddSkillCriticalRate(float value){skillInfo.AttackCriticalRate += value;}
};
