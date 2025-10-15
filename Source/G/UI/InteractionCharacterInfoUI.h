// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionCharacterInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class G_API UInteractionCharacterInfoUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UBorder* TextBorder;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;

	bool isOn = false;
public:
	void Init(FString string, FVector2D targetpos, FVector2D offset);
	void Active();
	void DeActive();
};
