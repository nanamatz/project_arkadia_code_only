// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Item/ItemBase.h"
#include "Ability.h"
#include "G/Util/Define.h"
#include "EquipItemBase.generated.h"


UENUM()
enum class EEquipType
{
	helmet UMETA(DisplayName = "helmet"),
	chestplate UMETA(DisplayName = "chestplate"),
	boots UMETA(DisplayName = "boots"),
	amulet UMETA(DisplayName = "amulet"),
	belt UMETA(DisplayName = "belt"),
	ring UMETA(DisplayName = "ring"),
	gloves UMETA(DisplayName = "gloves"),
	weapon UMETA(DisplayName = "weapon")
};

/**
 * 
 */
UCLASS()
class G_API UEquipItemBase : public UItemBase
{
	GENERATED_BODY()

protected:
	enum ECharacterName EquipCharName = ECharacterName::None; //장착한 캐릭터 인덱스 
	
	int EnhenceValue = 0; //강화 지수 

	UPROPERTY(EditAnywhere)
	EEquipType equip_type;

public:
	UPROPERTY(EditAnywhere)
	struct FAbility ability; //스텟

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<TSubclassOf<UItemBase>, int32> EnhenceMaterial; //강화에 필요한 아이템의 종류와 개수를 담은 Map

	UPROPERTY(EditAnyWhere)
	int32 EnhenceGold;

	virtual bool Equip(enum ECharacterName n); //장비 장착, 인수는 캐릭터 넘버
	virtual void Unequip();
	virtual void UnEquipUIOnly();
	ECharacterName GetEquipCharName() { return EquipCharName; } //장착한 캐릭터의 인덱스 리턴
	bool IsEquip(); //장착 여부 리턴.

	int GetEnhenceLevel() { return EnhenceValue; } //강화 레벨 리턴
	void EnhenceDefault(int n); //강화시키기, 인수는 강화할 수치(나중에 대성공 같은 거 생기려나...?)
	virtual void Enhence(); //강화 수치를 1 올린다

	virtual FString GetItemType() override { return FString("Equipment"); } //장비 타입을 FString으로 리턴
	virtual EEquipType GetEquipType() { return equip_type; }
};
