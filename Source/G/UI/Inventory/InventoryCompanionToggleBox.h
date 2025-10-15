// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Util/Define.h"
#include "InventoryCompanionToggleBox.generated.h"

class UHorizontalBox;
class UWidget;

/**
 * 
 */
UCLASS()
class G_API UInventoryCompanionToggleBox : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontal_box;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryCompanionToggleButton> bp_inventory_toggle_button;

	UPROPERTY()
	class UCharacterCaptureWidget* cur_ccw;

public:
	void AddToggleButton(class APlayerCharacter* player_character, enum ECharacterName* target_character,
	                     class UCharacterCaptureWidget* ccw);
	UCharacterCaptureWidget* GetCurrentCaptureWidget() const;
	TArray<UWidget*> GetChild();
	void Clear();
};
