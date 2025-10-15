// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Item/Ability.h"
#include "Engine/DataTable.h"
#include "G/Util/Define.h"
#include "SkillData/SkillStruct.h"
#include "SkillData/SkillTreeNode/UpgradeNodeStruct.h"
#include "SkillData/SkillType/SkillCastBuff.h"
#include "SkillData/SkillType/SkillCastDebuff.h"
#include "SkillTreeNodeStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct G_API FSkillTreeNodeStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPROPERTY()
	class USkillBase* skillClass = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//class USkillBase* MySKill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Num;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Tier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Info")
	FText NodeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Info")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Info")
	class UTexture2D* Thumbnail = nullptr;

	// 필요한 스킬 노드 번호들(이전 티어의 스킬 번호들임)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> SkillTreeNodeRequirement;

	UPROPERTY(EditAnywhere, Category = "Requirement")
	FAbility AbilityRequirement;
	UPROPERTY(EditAnywhere, Category = "Requirement")
	int32 SkillPointRequirement = 1;
	UPROPERTY(EditAnywhere, Category = "Requirement")
	bool IsLearning = false;
	/* if requirement met, then parent make it true #1# */
	UPROPERTY(EditAnywhere)
	TArray<bool> isRequirementMet;

	UPROPERTY(EditAnywhere, Category = "SkillInfo Upgrade Info")
	FUpgradeNodeStruct UpgradeSkillInfo;
};
