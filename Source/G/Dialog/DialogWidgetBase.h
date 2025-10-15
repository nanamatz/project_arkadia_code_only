// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class G_API UDialogWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Accept();

	UFUNCTION(BlueprintImplementableEvent)
	void ChooseUp();

	UFUNCTION(BlueprintImplementableEvent)
	void ChooseDown();

	UFUNCTION(BlueprintImplementableEvent)
	void AcceptMouse();

	UFUNCTION(BlueprintImplementableEvent)
	void Cancel();

	UPROPERTY(BlueprintReadOnly)
	class ADialogManageActor* OwnerActor;
};
