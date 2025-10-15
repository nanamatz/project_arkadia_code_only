// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Util/Define.h"
#include "PlayerPortrait.generated.h"

class UBorder;
class UImage;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class G_API UPlayerPortrait : public UUserWidget
{
	GENERATED_BODY()

protected:
	enum ECharacterName character_name;

	UPROPERTY(meta = (BindWidget))
	UBorder* border;

	UPROPERTY(meta = (BindWidget))
	UImage* portrait;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* hp;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* hp_text;

public:
	void Init(class APlayerCharacter* pc);
	void SetHP(float curr_hp, float max_hp);
	void UpdateHP();
	void UpdateHP(ECharacterName characterName);
	ECharacterName GetCharName() { return character_name; }
};
