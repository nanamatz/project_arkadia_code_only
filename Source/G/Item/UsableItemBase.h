// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Item/ItemBase.h"
#include "G/Util/Define.h"
#include "UsableItemBase.generated.h"

class AGCharacter;
class APlayerCharacter;
class USkillBase;

/**
 * 
 */
UCLASS()
class G_API UUsableItemBase : public UItemBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillBase> Skill;

	UPROPERTY(EditAnywhere)
	EConsumableItemName EConsumableItemName;
	
	
protected:
	UUsableItemBase();

public:
	enum EConsumableItemName GetEConsumableItemName() const { return EConsumableItemName; }
	
	virtual void Use(AGCharacter* Character);
	virtual void Use(APlayerCharacter* Character);
	TSubclassOf<USkillBase> GetSkill();
};
