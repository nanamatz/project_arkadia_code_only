// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "OrderedButton.generated.h"

/**
 * 
 */
UCLASS()
class G_API UOrderedButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* button;

	/* Button Order in Toggle Box */
	int32 order;

public:
	UButton*& GetButton();
	void SetOrder(int32 _order);
	int32 GetOrder();
};
