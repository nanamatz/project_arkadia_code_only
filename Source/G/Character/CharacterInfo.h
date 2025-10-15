#pragma once

#include "CoreMinimal.h"
#include "LevelUpInfo.h"
#include "Engine/DataTable.h"
#include "G/Player/CharacterInfoStruct.h"
#include "CharacterInfo.generated.h"

USTRUCT(BlueprintType)
struct G_API FCharacterInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	ECombatNPCName ENPCName;

	UPROPERTY(EditAnywhere)
	ECharacterName EName;

	UPROPERTY(EditAnywhere)
	class UTexture2D* Thumbnail = nullptr;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 MaxLevel = 20;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 cur_exp = 0;
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 max_exp = 20;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 SkillPoint = 0;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 MaxHP = 10;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo")
	int32 CurHP;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 MaxCost = 3;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo")
	int32 CurCost;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	float MovePerCost = 10.0f;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo")
	float MoveCost;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 DefaultShield;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo")
	int32 CurShield;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 MinDamage = 0;
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	int32 MaxDamage = 0;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	FVector CharacterLocation = FVector(10418.671875f, -4746.746094f, 200);
	
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	float AvoidRate = 5.0f;
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	float HitRate = 100.0f;
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	float CriticalRate = 10.0f;
	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	float CriticalDamage = 150.0f;

	UPROPERTY(EditAnywhere, Category = "LevelInfo")
	struct FLevelUpInfo LevelUpInfo;

	UPROPERTY(EditAnywhere, Category = "Ability")
	EAbility ImportantAbility;

	UPROPERTY(EditAnywhere, Category = "Ability")
	int32 BonusValue = 0;

	UPROPERTY(EditAnywhere, Category = "Ability")
	struct FAbility Ability;

	UPROPERTY()
	bool BDeath = false;

	UPROPERTY()
	bool BInvulnerability = false;

	UPROPERTY()
	TArray<struct FBUFF> BuffList;
	UPROPERTY()
	TArray<struct FDEBUFF> DeBuffList;

	UPROPERTY()
	TMap<EEquipType, class UEquipItemBase*> equipped_item_map;
	
	UPROPERTY()
	int32 EnhenceValue; 
};
