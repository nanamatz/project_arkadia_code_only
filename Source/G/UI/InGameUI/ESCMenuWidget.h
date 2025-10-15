// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESCMenuWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class G_API UESCMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* resume_button;

	UPROPERTY(meta = (BindWidget))
	UButton* save_button;

	UPROPERTY(meta = (BindWidget))
	UButton* setting_button;

	UPROPERTY(meta = (BindWidget))
	UButton* exit_button;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void resume();

	UFUNCTION()
	void save();

	UFUNCTION()
	void setting();

	UFUNCTION()
	void exit();

public:
	UFUNCTION()
	void On();
	
	void Off();
};
