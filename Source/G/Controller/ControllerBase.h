// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class G_API UControllerBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UGGameInstance* GameInstance;

public:
	void Init(class UGGameInstance* gameInstance);
};
