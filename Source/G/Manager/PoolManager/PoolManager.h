// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Manager/ManagerBase.h"
#include "PoolManager.generated.h"

/**
 * 
 */
UCLASS()
class G_API UPoolManager : public UManagerBase
{
	GENERATED_BODY()
	UPROPERTY()
	TMap<FString, class UPool*> _pool;

public:
	virtual void Init(UGGameInstance* gameInstance) override;

	void CreatePool(TSubclassOf<AActor> original, int count = 5);

	void Push(class UPoolable* poolable);
	class UPoolable* Pop(TSubclassOf<AActor> original);
	TSubclassOf<AActor> GetOriginal(FString name);
	void Clear();
};
