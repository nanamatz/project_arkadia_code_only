// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuBase.generated.h"

/**
 * 
 */

UCLASS()
class G_API UGameMenuBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void On();
	virtual void Off();
};
