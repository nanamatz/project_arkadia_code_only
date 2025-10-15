// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Poolable.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class G_API UPoolable : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPoolable();
	bool IsUsing;
};
