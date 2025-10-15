// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointOfInterestComponent.generated.h"

class UTexture2D;
class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class G_API UPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* icon_image;

public:
	// Sets default values for this component's properties
	UPointOfInterestComponent();

	UPROPERTY(EditAnywhere)
	bool is_static;

	bool is_register = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UTexture2D* GetIconImage();
	void AddPOI();
	void SetIcon(UTexture2D* texture);
};
