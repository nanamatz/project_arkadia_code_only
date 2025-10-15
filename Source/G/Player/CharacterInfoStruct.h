// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <G/Util/Define.h>
#include "Engine/DataTable.h"
#include "G/Item/Ability.h"
#include "G/Item/EquipItemBase.h"
#include "CharacterInfoStruct.generated.h"


//UENUM()
//enum CharacterName {
//	Cardenio,
//	Fernando,
//	Samira,
//	RuthCinda
//};

USTRUCT(BlueprintType)
struct G_API FCharacterInitInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Name; // �̸�

	UPROPERTY(EditAnywhere)
	class UTexture2D* Thumbnail = nullptr; // �����

	UPROPERTY(EditAnywhere)
	int32 Level; // ����

	UPROPERTY(EditAnywhere)
	float DefaultHP; // ü��

	UPROPERTY(EditAnywhere)
	int DefaultCost; // �ڽ�Ʈ

	UPROPERTY(EditAnywhere)
	float DefaultArmor; // �Ƹ�

	UPROPERTY(EditAnywhere)
	float DefaulDamage; // ���ݷ�

	UPROPERTY(EditAnywhere)
	float LevelArmor; // ���� �� ������ �������� �߰� �Ƹ�

	UPROPERTY(EditAnywhere)
	float LevelDamage; // ���� �� ������ �������� �߰� ���ݷ�

	UPROPERTY(EditAnywhere)
	float LevelHP; // ���� �� ������ �������� �߰� ü��

	UPROPERTY(EditAnywhere)
	float LevelCost; // ���� �� ������ �������� �߰� �ڽ�Ʈ

	UPROPERTY(EditAnywhere)
	float AttackRange; // ���� ��Ÿ�

	UPROPERTY(EditAnywhere)
	float strength;
	UPROPERTY(EditAnywhere)
	float insight;
	UPROPERTY(EditAnywhere)
	float swift;
	UPROPERTY(EditAnywhere)
	float relaxed;
	UPROPERTY(EditAnywhere)
	float careful;
	UPROPERTY(EditAnywhere)
	float luck;
};

USTRUCT(BlueprintType)
struct FBUFF
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadOnly)
	EBUFFTYPE type;
	UPROPERTY(BlueprintReadOnly)
	int32 turnCount;
	UPROPERTY(BlueprintReadOnly)
	int32 buffAmount;
};

USTRUCT(BlueprintType)
struct FDEBUFF
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadOnly)
	EDEBUFFTYPE type;
	UPROPERTY(BlueprintReadOnly)
	int32 turnCount;
	UPROPERTY(BlueprintReadOnly)
	int32 buffAmount;
};

/*
*
*	��ȭ�ϴ� ĳ���� ����
*
*/
USTRUCT()
struct FCharacterInfoStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Name; // �̸�

	UPROPERTY(EditAnywhere)
	class UTexture2D* Thumbnail = nullptr; // �����

	UPROPERTY(EditAnywhere)
	int32 Level; // ����

	float temp_exp_percent = 0; // To provide zero divide error in test version

	UPROPERTY(EditAnywhere)
	float cur_exp;

	UPROPERTY(EditAnywhere)
	float max_exp;

	UPROPERTY(EditAnywhere)
	float MaxHP; // �ִ� ü��

	UPROPERTY(EditAnywhere)
	float CurHP; // ���� ü��, ���� ������� �ݿ���

	UPROPERTY(EditAnywhere)
	int MaxCost; // �ִ� �ڽ�Ʈ

	UPROPERTY(EditAnywhere)
	int CurCost; // ���� �ڽ�Ʈ, ���� ������� �ݿ���

	UPROPERTY(EditAnywhere)
	float MoveCost;

	UPROPERTY(EditAnywhere)
	float DefaultArmor; // �⺻ ����

	UPROPERTY(EditAnywhere)
	float CurArmor; // ���� ����, ���� ������� �ݿ��� 

	UPROPERTY(EditAnywhere)
	float DefaultDamage; // �⺻ ���ݷ� : ��ų �������� �߰������� ���� ������

	UPROPERTY(EditAnywhere)
	float CurDamage; // ���� ���ݷ�, ���� ������� �ݿ���

	UPROPERTY(EditAnywhere)
	bool BDeath; // �׾����� Ȯ��

	UPROPERTY(EditAnywhere)
	float AttackRange; // ���� ��Ÿ�	

	UPROPERTY(EditAnywhere)
	struct FAbility ability;

	UPROPERTY()
	TMap<EEquipType, class UEquipItemBase*> equipped_item_map; // ������ ������

	TArray<struct FBUFF*> buffList;
	TArray<struct FDEBUFF*> deBuffList;
};
