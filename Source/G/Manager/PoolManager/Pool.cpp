// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool.h"

#include "Poolable.h"

void UPool::Init(UGameInstance* gameInstance, TSubclassOf<AActor> original, int count)
{
	_gameInstance = gameInstance;
	Original = original;
	for (int i = 0; i < count; i++)
	{
		Push(Create());
	}
}

void UPool::Push(UPoolable* poolable)
{
	if (poolable == nullptr)
	{
		return;
	}

	poolable->GetOwner()->SetHidden(true);
	poolable->IsUsing = false;

	_poolStack.Push(poolable);
}

UPoolable* UPool::Pop()
{
	UPoolable* poolable = NewObject<UPoolable>();
	if (_poolStack.Num() > 0)
	{
		poolable = _poolStack.Pop();
	}
	else
	{
		poolable = Create();
	}

	poolable->GetOwner()->SetHidden(false);
	poolable->IsUsing = true;

	return poolable;
}

UPoolable* UPool::Create()
{
	AActor* actor = _gameInstance->GetWorld()->SpawnActor<AActor>(Original.Get());
	auto component = actor->K2_GetComponentsByClass(UPoolable::StaticClass());
	UPoolable* NewUPoolable = Cast<UPoolable>(component[0]);
	return NewUPoolable;
}
