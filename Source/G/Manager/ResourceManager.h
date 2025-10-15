// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/GGameInstance.h"
#include "PoolManager/Poolable.h"
#include "PoolManager/PoolManager.h"
#include "ResourceManager.generated.h"

/**
 * 
 */
UCLASS()
class G_API UResourceManager : public UManagerBase
{
	GENERATED_BODY()

public:
	virtual void Init(UGGameInstance* gameInstance) override;
	void ObjectDestory(AActor* actor);

	template <typename T>
	T* Instantiate(TSubclassOf<T> original);
};

template <typename T>
T* UResourceManager::Instantiate(TSubclassOf<T> original)
{
	auto components = Cast<AActor>(original.GetDefaultObject())->K2_GetComponentsByClass(UPoolable::StaticClass());
	if (components.Num() >= 1)
	{
		// 오브젝트 풀에서 뽑아오기
		return Cast<T>(GameInstance->GetPoolManager()->Pop(original)->GetOwner());
	}
	T* actor = GameInstance->GetWorld()->SpawnActor<T>(original.Get());
	return actor;
}
