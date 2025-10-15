// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBox.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class G_API UStatBox : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* power_value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* armor_value;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* strength_value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* insight_value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* swift_value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* relaxed_value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* careful_value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* luck_value;

public:
	void Update(struct FAbility& ability);
};
