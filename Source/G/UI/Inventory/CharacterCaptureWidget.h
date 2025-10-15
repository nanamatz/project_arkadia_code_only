// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCaptureWidget.generated.h"

class UEquipItemBase;
class UImage;

/**
 * 
 */
UCLASS()
class G_API UCharacterCaptureWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* helmet_img;

	UPROPERTY(meta = (BindWidget))
	UImage* chestplate_img;

	UPROPERTY(meta = (BindWidget))
	UImage* belt_img;

	UPROPERTY(meta = (BindWidget))
	UImage* amulet_img;

	UPROPERTY(meta = (BindWidget))
	UImage* ring_img;

	UPROPERTY(meta = (BindWidget))
	UImage* boots_img;

	UPROPERTY(meta = (BindWidget))
	UImage* gloves_img;

	UPROPERTY(meta = (BindWidget))
	UImage* weapon_img;

public:
	void Update(TMap<enum class EEquipType, UEquipItemBase*>& equip_items);
	void SetHelmet(class UTexture2D* icon);
	void SetChestPlate(class UTexture2D* icon);
	void SetBelt(class UTexture2D* icon);
	void SetAmulet(class UTexture2D* icon);
	void SetRing(class UTexture2D* icon);
	void SetBoots(class UTexture2D* icon);
	void SetGloves(class UTexture2D* icon);
	void SetWeapon(class UTexture2D* icon);

	void UnsetHelmet();
	void UnsetChestPlate();
	void UnsetBelt();
	void UnsetAmulet();
	void UnsetRing();
	void UnsetBoots();
	void UnsetGloves();
	void UnsetWeapon();
};
