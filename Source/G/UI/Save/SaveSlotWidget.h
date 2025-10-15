// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSlotWidget.generated.h"

class UButton;
class UTextBlock;
class USaveSlotWidget;
class UEditableText;
/**
 * 
 */
UCLASS()
class G_API USaveSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* slot_bt;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* slot_text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* date_text;

	USaveSlotWidget** selected_slot_ptr;

	UEditableText** save_name_text_ptr;
	
private:
	UFUNCTION()
	void SetSelectedSlot();
	
protected:
	virtual bool Initialize() override;
	
public:
	void Set(UEditableText** _selected_slot_ptr, FString& path, FDateTime& modified_date);
};
