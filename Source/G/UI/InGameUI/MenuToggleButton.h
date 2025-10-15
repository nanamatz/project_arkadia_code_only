// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/IndexButton.h"
#include "MenuToggleButton.generated.h"

/**
 * 
 */
UCLASS()
class G_API UMenuToggleButton : public UIndexButton
{
	GENERATED_BODY()

	UFUNCTION()
	void OnGameMenu();
	
public:
	UMenuToggleButton();
};
