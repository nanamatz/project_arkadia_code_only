#pragma once
#include "G/Util/Define.h"
#include "SkillCastAttack.generated.h"

USTRUCT(BlueprintType)
struct FSkillCastAttack
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EAttackType Type = EAttackType::None;

	UPROPERTY(EditAnywhere)
	int32 MinDamage = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxDamage = 0;


	UPROPERTY(EditAnywhere)
	// 생성물 지속 시간
	int32 Duration = 0;

	int32 GetRandomDamage() const { return FMath::RandRange(MinDamage, MaxDamage); }
};
