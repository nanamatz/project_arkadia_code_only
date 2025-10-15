// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/PlayerCharacter.h"
#include "FernandoClass.generated.h"


/**
 *
 * 캐릭터의 스킬들 넣기
 * FernandoClass
 *
 */

UCLASS()
class G_API AFernandoClass : public APlayerCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AFernandoClass();
};
