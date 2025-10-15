// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Util/Define.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

UCLASS()
class G_API ADamageTextActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageTextActor();

protected:
	UPROPERTY(VisibleAnywhere, Category="TextWidget", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* DamageTextWidgetComponent;

	/*UPROPERTY(VisibleAnywhere)
	class UPoolable* PoolableComponent;
	*/

	/*virtual void BeginDestroy() override;*/

	FVector startLocation;
	FVector endLocation;

public:
	FTimerHandle destroyTimerHandle;

	void Init(FVector _startLocation, float _damage, EDamageType damageType);
	void Init(FVector _startLocation, FText text);
};
