// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G.h"
#include "G/Player/CharacterInfoStruct.h"
#include "G/Character/CharacterInfo.h"
#include "CharacterInfoComponent.generated.h"

/**
 * 
 */
UCLASS()
class G_API UCharacterInfoComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	struct FCharacterInfo characterInfo;

public:
	void InitCharacterInfo();

	void SetCharacterInfo(FCharacterInfo info, bool bStart = true);
	FCharacterInfo& GetCharacterInfo() { return characterInfo; }

	// 데미지 받기
	virtual void Death() { characterInfo.BDeath = true; }
	int32 GetLevel() const { return characterInfo.Level; }
	int32 GetCurExp() const { return characterInfo.cur_exp; }
	int32 GetMaxExp() const { return characterInfo.max_exp; }

	float GetExpPercent() const
	{
		return (static_cast<float>(characterInfo.cur_exp) / static_cast<float>(characterInfo.max_exp));
	}

	int32 GetSkillPoint() const { return characterInfo.SkillPoint; }
	int32 GetCurrentHP() const { return characterInfo.CurHP; }
	int32 GetMaxHP() const { return characterInfo.MaxHP; }
	int32& GetCurrentHPRef() { return characterInfo.CurHP; }
	int32& GetMaxHPRef() { return characterInfo.MaxHP; }
	int32 GetCurrentCost() const { return characterInfo.CurCost; }
	int32 GetMaxCost() const { return characterInfo.MaxCost; }
	float GetMoveCost() const { return characterInfo.MoveCost; }
	float GetMovePerCost() const { return characterInfo.MovePerCost; }
	int32 GetCurrentShield() const { return characterInfo.CurShield; }
	FVector GetLocation() const {return characterInfo.CharacterLocation;}
	int32 GetMinDamage() const { return characterInfo.MinDamage; }
	int32 GetMaxDamage() const { return characterInfo.MaxDamage; }
	int32 GetRandomDamage() const { return FMath::RandRange(characterInfo.MinDamage, characterInfo.MaxDamage); }
	int32 GetDamage();
	bool GetBDeath() const { return characterInfo.BDeath; }
	bool GetBInvulnerability() const {return characterInfo.BInvulnerability; }
	FText GetCharacterName() const { return characterInfo.Name; }
	UTexture2D& GetPortrait() const { return *(characterInfo.Thumbnail); }
	TArray<struct FBUFF>& GetBuffListRef() { return characterInfo.BuffList; }
	TArray<struct FDEBUFF>& GetDeBuffListRef() { return characterInfo.DeBuffList; }
	struct FAbility& GetAbilityRef() { return characterInfo.Ability; }
	int32 GetAbilityValue(EAbility type) const { return characterInfo.Ability.Value[type]; }
	ECharacterName GetECharacterName() const { return characterInfo.EName; }
	float GetAvoidRate() const { return characterInfo.AvoidRate + characterInfo.Ability.Value[EAbility::Swift]; }
	float GetHitRate() const { return characterInfo.HitRate + characterInfo.Ability.Value[EAbility::Careful]; }
	
	float GetCriticalRate() const
	{
		return characterInfo.CriticalRate + characterInfo.Ability.Value[EAbility::Relaxed];
	}

	float GetCriticalDamage() const
	{
		return (characterInfo.CriticalDamage + (characterInfo.Ability.Value[EAbility::Luck] * 5.0f)) / 100.0f;
	}

	void SetCurrentHP(float value) { characterInfo.CurHP = value; }
	void SetBDeath(bool _b) { characterInfo.BDeath = _b; }
	void SetBInvulnerability(bool b) {characterInfo.BInvulnerability = b;}
	void SetMoveCost(float value = 0.0f) { characterInfo.MoveCost = value; }
	void SetLocation(const FVector& location) { characterInfo.CharacterLocation = location; }

	// 능력치 맥스값 99999
	template <typename T>
	void AddValue(T& value, T& addValue, T maxValue = 99999, T minValue = 0);
	int32 AddSkillPoint(int32 value) { AddValue(characterInfo.SkillPoint, value); return characterInfo.SkillPoint; }
	void AddCurrentHP(int32 _num) { AddValue(characterInfo.CurHP, _num, characterInfo.MaxHP); }
	void AddMaxHP(int32 value) { AddValue(characterInfo.MaxHP, value); }
	void AddCurrentCost(int32 _num) { AddValue(characterInfo.CurCost, _num, characterInfo.MaxCost); }
	void AddMaxCost(int32 value) { AddValue(characterInfo.MaxCost, value); }
	void AddMovePerCost(float value) { AddValue(characterInfo.MovePerCost, value); }
	void AddMoveCost(float value)
	{
		AddValue(characterInfo.MoveCost, value);
	}
	void AddCurrentShield(int32 value) { AddValue(characterInfo.CurShield, value); }
	void AddCurrentExp(int32 value);

	void AddCurrentStrength(float value) { characterInfo.Ability.Value[EAbility::Strength] += value; }
	void AddCurrentInsight(float value) { characterInfo.Ability.Value[EAbility::Insight] += value; }
	void AddCurrentLuck(float value) { characterInfo.Ability.Value[EAbility::Luck] += value; }
	void AddCurrentRelaxed(float value) { characterInfo.Ability.Value[EAbility::Relaxed] += value; }
	void AddCurrentCareful(float value) { characterInfo.Ability.Value[EAbility::Careful] += value; }
	void AddCurrentSwift(float value) { characterInfo.Ability.Value[EAbility::Swift] += value; }
	void AddBuffList(struct FBUFF newBuff);
	void AddDeBuffList(struct FDEBUFF newDeBuff);

	void AddAvoidRate(float value) { AddValue(characterInfo.AvoidRate, value); }
	void AddHitRate(float value) { AddValue(characterInfo.HitRate, value); }
	void AddCriticalRate(float value) { AddValue(characterInfo.CriticalRate, value); }
	void AddCriticalDamage(float value) { AddValue(characterInfo.CriticalDamage, value); }

	void CountBuffAndDeBuff();
	void LevelUp();
	//int32 CalculateAbility();
};
