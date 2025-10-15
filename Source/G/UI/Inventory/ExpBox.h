// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpBox.generated.h"

/**
 * 
 */
UCLASS()
class G_API UExpBox : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* level_text;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* exp_bar;

public:
	void Update(int32& level, float exp_percent);
	void SetExp(float exp_percent);
	void SetLevel(int32& level);
};
