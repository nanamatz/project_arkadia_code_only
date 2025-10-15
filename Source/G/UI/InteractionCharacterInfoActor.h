// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionCharacterInfoActor.generated.h"

UCLASS()
class G_API AInteractionCharacterInfoActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractionCharacterInfoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InteractionCharacterInfoWidgetComponent;

	FString CharName;
	bool Initialization = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Active();
	void DeActive();

	void Init(FVector, FString);
};
