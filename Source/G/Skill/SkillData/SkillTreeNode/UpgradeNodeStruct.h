#pragma once
#include "CoreMinimal.h"
#include <G/Util/Define.h>

#include "G/Skill/SkillData/SkillType/SkillCastAttack.h"
#include "G/Skill/SkillData/SkillType/SkillCastBuff.h"
#include "G/Skill/SkillData/SkillType/SkillCastDebuff.h"
#include "UpgradeNodeStruct.generated.h"


USTRUCT(BlueprintType)
struct G_API FUpgradeNodeStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* skillAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Info")
	class UTexture2D* Thumbnail = nullptr;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 CoolDown = 0;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 Cost = 0;

	UPROPERTY(EditAnywhere, Category = "Skill Target Type")
	ESkillTargetType SkillTargetType = ESkillTargetType::NONE;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	ESkillRangeType SkillRangeType = ESkillRangeType::None;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	float CharacterRange = 0;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	float MouseRange = 0;

	UPROPERTY(EditAnywhere, Category = "Damage")
	EAttackType ChangeAttackType = EAttackType::None;

	UPROPERTY(EditAnywhere, Category = "Damage")
	// Attack 타입 데미지 증가
	int32 UpgradeDamage = 0;

	UPROPERTY(EditAnywhere, Category = "Damage")
	// Attack 타입 중 생성물 지속시간 증가
	int32 UpgradeDuration = 0;

	UPROPERTY(EditAnywhere, Category = "Attack Type")
	TArray<struct FSkillCastAttack> AttackList;

	UPROPERTY(EditAnywhere, Category = "Attack Type")
	float AddSkillCriticalRate;
	
	UPROPERTY(EditAnywhere, Category = "Buff Type")
	TArray<struct FSkillCastBuff> BuffList;

	UPROPERTY(EditAnywhere, Category = "Debuff Type")
	TArray<struct FSkillCastDebuff> DebuffList;

	UPROPERTY(EditAnywhere, Category = "Action Type")
	TArray<TSubclassOf<class UActionBase>> ActionList;
};
