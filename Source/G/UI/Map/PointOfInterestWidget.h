// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointOfInterestWidget.generated.h"

class UTexture2D;
class UThrobber;
class UImage;
class AActor;

/**
 * 
 */
UCLASS()
class G_API UPointOfInterestWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* custom_image;

	UPROPERTY(meta = (BindWidget))
	UThrobber* default_image;

	UPROPERTY()
	AActor* owner;

	FString owner_name;
	
public:
	bool IsStatic;

	void SetOwner(AActor* _owner);
	AActor* GetOwner() { return owner; }
	FString GetOwnerName() { return owner_name; }	
};
