#pragma once
#include "G/Util/Define.h"
#include "SkillCastBuff.generated.h"

USTRUCT(BlueprintType)
struct FSkillCastBuff
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EBUFFTYPE type;
	UPROPERTY(EditAnywhere)
	int32 Value;
	UPROPERTY(EditAnywhere)
	int32 Duration;
};
