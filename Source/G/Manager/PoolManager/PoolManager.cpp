// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolManager.h"

#include "Pool.h"
#include "Poolable.h"
#include "G/G.h"
#include "G/GGameInstance.h"

void UPoolManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

void UPoolManager::CreatePool(TSubclassOf<AActor> original, int count)
{
	LOG(Log, "%s CreatePool", *original->GetName());
	UPool* pool = NewObject<UPool>(this, UPool::StaticClass());
	pool->Init(GameInstance, original, count);
	_pool.Add(original->GetName(), pool);
}

void UPoolManager::Push(UPoolable* poolable)
{
	FString name = poolable->GetOwner()->GetClass()->GetName();
	if (_pool.Contains(name) == false)
	{
		LOG(Log, "%s Destroy", *name);
		poolable->GetOwner()->Destroy();
		return;
	}
	LOG(Log, "%s PUSH", *name);
	_pool[name]->Push(poolable);
}

UPoolable* UPoolManager::Pop(TSubclassOf<AActor> original)
{
	FString name = original->GetName();
	if (_pool.Contains(name) == false)
	{
		CreatePool(original);
	}
	return _pool[name]->Pop();
}

TSubclassOf<AActor> UPoolManager::GetOriginal(FString name)
{
	if (_pool.Contains(name) == false)
	{
		return nullptr;
	}

	return _pool[name]->Original;
}

void UPoolManager::Clear()
{
	_pool.Empty();
}
