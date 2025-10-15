// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pool.generated.h"

/**
 * 
 */
UCLASS()
class G_API UPool : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSubclassOf<AActor> Original;

	UPROPERTY()
	TArray<class UPoolable*> _poolStack;

	UPROPERTY()
	UGameInstance* _gameInstance;

	void Init(class UGameInstance* gameInstance, TSubclassOf<AActor> original, int count = 5);

	void Push(class UPoolable* poolable);

	class UPoolable* Pop();

private:
	UPoolable* Create();
};
