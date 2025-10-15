// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSystemUI.generated.h"

class UImage;
class UButton;
class UScrollBox;
class UEditableText;
class USaveSlotWidget;

/**
 * 
 */
UCLASS()
class G_API USaveSystemUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* rep_image;

	UPROPERTY(meta = (BindWidget))
	UButton* save_bt;

	UPROPERTY(meta = (BindWidget))
	UButton* load_bt;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* save_scroll_box;

	UPROPERTY(meta = (BindWidget))
	UEditableText* save_name_text;

	UPROPERTY(meta = (BindWidget))
	UButton* exit_bt;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USaveSlotWidget> bp_slot_widget;

	
	
private:
	UFUNCTION()
	void Save();

	UFUNCTION()
	void Load();
	
protected:
	virtual bool Initialize() override;

public:
	void On();
	void On(bool save = true);
	UFUNCTION()
	void Off();

	void AddSavePath(FString path);
	void AddSavePath(TArray<FString> paths);

	UFUNCTION()
	void SaveForce();

	UFUNCTION()
	void SaveForceCancel();
};
