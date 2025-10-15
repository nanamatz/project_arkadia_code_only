// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableItemBase.h"
#include "PotionBase.generated.h"

class APlayerCharacter;

/**
 * 	
 */
UCLASS()
class G_API UPotionBase : public UUsableItemBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 HP_Increase = 0;

protected:
	UPotionBase();

public:
	virtual void Use(APlayerCharacter* Character) override;
};
