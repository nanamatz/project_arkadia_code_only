// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Util/Define.h"
#include "G/Manager/ManagerBase.h"
#include "DataManager.generated.h"

/**
 * 
 */

UCLASS()
class G_API UDataManager : public UManagerBase
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FString, class UDataTable*> _dataTableMap;

	UPROPERTY()
	TMap<FString, TSubclassOf<class APlayerCharacter>> _bpPlayerClass;

public:
	UDataManager();

	virtual void Init(UGGameInstance* gameInstance) override;

	TSubclassOf<class APlayerCharacter> GetPlayerCharacterBP(ECharacterName characterName);

	template <typename T>
	T* GetDataTable(EDataType dataType, int rowNum);
	template <typename T>
	T* GetDataTable(EDataType dataType, FString& rowName);

	struct FCharacterInfo* GetPlayerCharacterInfoData(int rowNum);
	struct FCharacterInfo* GetPlayerCharacterInfoData(ECharacterName rowName);
	struct FSkillStruct* GetSkillData(EDataType characterName, int rowNum);
	struct FSkillStruct* GetSkillData(EDataType dataType, FString rowName);
	struct FTutorialType* GetTutorialData(EDataType dataType, FString rowName);
	
	class UDataTable* GetSkillDataTable(ECharacterName characterName);
	UDataTable* GetNPCSkillDataTable(const FString& characterName);
	struct FCharacterInfo* GetNPCCharacterInfo(FString& rowName);

	void AddDataTable(EDataType eName, UDataTable* dataTable);
	void AddDataTable(const FString& name, UDataTable* dataTable);
	void AddDataTable(EDataType eName, const FString& name);
};
