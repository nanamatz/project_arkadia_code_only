// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "HorizontalComboBox.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS(Category="ComboBox")
class G_API UHorizontalComboBox : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FText> default_options;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* selected_option;

	UPROPERTY(meta = (BindWidget))
	UButton* left_button;

	UPROPERTY(meta = (BindWidget))
	UButton* right_button;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* hb;

	UPROPERTY(EditAnywhere)
	int32 selected_idx = 0;

	int32 set_idx = 0;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void Prev();

	UFUNCTION()
	void Next();

public:
	FText GetSelectedOption();
	int32 GetSelectedOptionIdx();
	void SetDefaultOptions(TArray<FText>& option_list);
	void AddDefaultOption(FText option);
	void ClearDefaultOptions();
	void SetSelectedOption(FText option);
	void SetSelectedOption(int32 idx);
	void Apply();
};
