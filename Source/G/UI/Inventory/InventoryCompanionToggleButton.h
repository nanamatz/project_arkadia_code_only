// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/OrderedButton.h"
#include "G/Util/Define.h"
#include "InventoryCompanionToggleButton.generated.h"

class UButton;
class UCharacterCaptureWidget;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class G_API UInventoryCompanionToggleButton : public UOrderedButton
{
	GENERATED_BODY()
	ECharacterName bt_target_character;
	ECharacterName* target_character;

	UPROPERTY()
	UCharacterCaptureWidget* ccw;

	UCharacterCaptureWidget** cur_ccw;

	/* function */
	UFUNCTION()
	void OnClicked();

public:
	void Init(APlayerCharacter* player_character, enum ECharacterName* target_player,
	          UCharacterCaptureWidget* _ccw, UCharacterCaptureWidget** _cur_ccw);

	void SetInventoryTarget();
	
	enum ECharacterName GetTargetCharacterName() { return bt_target_character; }
};
