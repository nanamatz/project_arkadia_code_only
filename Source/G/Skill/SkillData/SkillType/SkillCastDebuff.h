#pragma once
#include "G/Util/Define.h"
#include "SkillCastDebuff.generated.h"

USTRUCT(BlueprintType)
struct FSkillCastDebuff
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	EDEBUFFTYPE type;
	UPROPERTY(EditAnywhere)
	int32 Value;
	UPROPERTY(EditAnywhere)
	int32 Duration;
};
