// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include <G/Util/Define.h>

#include "SkillType/SkillCastAttack.h"
#include "SkillType/SkillCastBuff.h"
#include "SkillType/SkillCastDebuff.h"
#include "SkillStruct.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct G_API FSkillStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USkillBase> skillClass;

	UPROPERTY(EditAnywhere)
	class UDataTable* skillTreeData = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* skillAnimMontage = nullptr;


	UPROPERTY(EditAnywhere, Category = "Info")
	class UTexture2D* Thumbnail = nullptr;

	UPROPERTY(EditAnywhere, Category = "Info")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "Info")
	FText Description;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 CoolDown;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 Cost;

	UPROPERTY(EditAnywhere, Category = "Info")
	float SkillTiming = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Skill Type")
	ESkillType SkillType = ESkillType::TARGETING;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	ESkillRangeType SkillRangeType; // 스킬 범위 타입(캐릭터 중심 원, 마우스 중심 원, 캐릭터 중심 부채꼴)

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	float CharacterRange; // 캐릭터부터의 범위

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	float MouseRange; // 마우스에서 범위 0인 경우 타겟팅

	UPROPERTY(EditAnywhere, Category = "Skill Target Type")
	ESkillTargetType TargetType;

	UPROPERTY(EditAnywhere, Category = "Attack Type")
	TArray<struct FSkillCastAttack> AttackList;

	UPROPERTY(EditAnywhere, Category = "Attack Type")
	float AttackCriticalRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Buff Type")
	TArray<struct FSkillCastBuff> BuffList;

	UPROPERTY(EditAnywhere, Category = "Debuff Type")
	TArray<struct FSkillCastDebuff> DebuffList;

	UPROPERTY(EditAnywhere, Category = "Action Type")
	TArray<TSubclassOf<class UActionBase>> ActionClassList;

	UPROPERTY()
	TArray<class UActionBase*> ActionList;
};
