// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ManagerBase.generated.h"

/**
 * 
 */
UCLASS()
class G_API UManagerBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UGGameInstance* GameInstance;

	bool _init = false;;

public:
	virtual void Init(class UGGameInstance* gameInstance);
};
