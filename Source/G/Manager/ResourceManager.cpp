// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"

#include "G/G.h"
#include "G/GGameInstance.h"
#include "PoolManager/Poolable.h"
#include "PoolManager/PoolManager.h"

void UResourceManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

void UResourceManager::ObjectDestory(AActor* actor)
{
	if (actor == nullptr)
	{
		return;
	}

	auto components = actor->K2_GetComponentsByClass(UPoolable::StaticClass());
	LOG(Log, "components num : %d", components.Num());
	if (components.Num() > 0)
	{
		// 오브젝트 풀에 다시 넣기
		return GameInstance->GetPoolManager()->Push(Cast<UPoolable>(components[0]));
	}

	actor->Destroy();
}
