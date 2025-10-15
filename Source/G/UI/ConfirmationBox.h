// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmationBox.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
// 헤더 파일에 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmationResult, bool, bConfirmed);

UCLASS()
class G_API UConfirmationBox : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* text;
	
	UPROPERTY(meta = (BindWidget))
	UButton* confirm_bt;
	
	UPROPERTY(meta = (BindWidget))
	UButton* cancel_bt;

	bool res;
	
private:
	UFUNCTION()
	void Confirm();

	UFUNCTION()
	void Cancel();
	
protected:
	virtual bool Initialize() override;

public:
	// 델리게이트 정의
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnConfirmationResult OnConfirmResult;
	
	bool* On(FText _text, bool bCancel = true);
	UButton* GetConfirmButton() { return confirm_bt; }
	UButton* GetCancelButton() { return cancel_bt; }
};
