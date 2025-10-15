#include "G/Manager/DataManager.h"

#include "Engine/DataTable.h"
#include <G/Player/CharacterInfoStruct.h>

#include <G/PlayerCharacter.h>
#include "G/G.h"
#include "G/Skill/SkillData/SkillStruct.h"
#include "G/Tutorial/FTutorialType.h"

const FString NPCSkillDataTableName = "SkillData";
const FString DataTablePath = "/Game/Asset/DataTable";

UDataManager::UDataManager()
{
	AddDataTable(PlayerCharacterData, "DefaultPlayerCharacterData");
	AddDataTable(ConsumableItemSkillData, "ConsumableItemSkillData");

	AddDataTable(CardenioSkillData, "CardenioSkillData");
	AddDataTable(FernandoSkillData, "FernandoSkillData");
	AddDataTable(RuthCindaSkillData, "RuthCindaSkillData");
	AddDataTable(SamiraSkillData, "SamiraSkillData");

	AddDataTable(TutorialData, "TutorialData");
	
	// NPC 정보 데이터 테이블에서 가져오기
	static ConstructorHelpers::FObjectFinder<UDataTable> NPCDataTableFinder(
		TEXT("DataTable'/Game/CombatSystem/NPC/DataTable/DefaultNPCData.DefaultNPCData'"));
	if (NPCDataTableFinder.Succeeded())
	{
		UDataTable* dataTable = Cast<UDataTable>(NPCDataTableFinder.Object);
		if (dataTable)
		{
			AddDataTable(NPCData, dataTable);
			// 스킬 데이터 테이블도 가져오기
			TArray<FName> rowNames = dataTable->GetRowNames();

			for (ECombatNPCName type : TEnumRange<ECombatNPCName>())
			{
				const FString characterName = UEnum::GetDisplayValueAsText(type).ToString();
				const FString skillDataTablePath = "DataTable'/Game/CombatSystem/NPC/DataTable/SkillDataTable/NPC_SkillDataTable_"
				+ characterName + "." + "NPC_SkillDataTable_" + characterName + "'";

				LOG(Log, "characterName : %s", *characterName);
				LOG(Log, "skillDataTablePath : %s", *skillDataTablePath);
				ConstructorHelpers::FObjectFinder<UDataTable> NPCSkillDataTable(*skillDataTablePath);

				if (NPCSkillDataTable.Succeeded())
				{
					UDataTable* npcSkillDataTable = Cast<UDataTable>(NPCSkillDataTable.Object);
					if (npcSkillDataTable)
					{
						const FString tableName = characterName + NPCSkillDataTableName;
						AddDataTable(tableName, npcSkillDataTable);
					}
				}
			}
		}
	}


	// 플레이어 캐릭터 블루프린트
	for (ECharacterName type : TEnumRange<ECharacterName>())
	{
		ECharacterName eName = type;
		FString characterName = UEnum::GetDisplayValueAsText(eName).ToString();
		FString PlayerClassPath = FString::Printf(
			TEXT("/Game/Character/%s/BP_%sCharacter.BP_%sCharacter_C"), *characterName, *characterName, *characterName);
		ConstructorHelpers::FClassFinder<APlayerCharacter> FindBPPlayerClass(*PlayerClassPath);
		if (FindBPPlayerClass.Succeeded())
		{
			TSubclassOf<class APlayerCharacter> bpPlayerClass = FindBPPlayerClass.Class;
			_bpPlayerClass.Add(characterName, bpPlayerClass);
		}
		else
		{
		}
	}
}

void UDataManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

TSubclassOf<APlayerCharacter> UDataManager::GetPlayerCharacterBP(ECharacterName eName)
{
	FString characterName = UEnum::GetDisplayValueAsText(eName).ToString();
	if (auto& bpClass = _bpPlayerClass.FindChecked(characterName))
	{
		return bpClass;
	}
	return nullptr;
}


FCharacterInfo* UDataManager::GetPlayerCharacterInfoData(int rowNum)
{
	return GetDataTable<FCharacterInfo>(PlayerCharacterData, rowNum);
}

FCharacterInfo* UDataManager::GetPlayerCharacterInfoData(ECharacterName rowName)
{
	FString characterName = UEnum::GetDisplayValueAsText(rowName).ToString();
	LOG(Log, "%s : Get Player Character Info", *characterName);
	return GetDataTable<FCharacterInfo>(PlayerCharacterData, characterName);
}

FSkillStruct* UDataManager::GetSkillData(EDataType characterName, int rowNum)
{
	return GetDataTable<FSkillStruct>(characterName, rowNum);
}

FSkillStruct* UDataManager::GetSkillData(EDataType dataType, FString rowName)
{
	return GetDataTable<FSkillStruct>(dataType, rowName);
}

FTutorialType* UDataManager::GetTutorialData(EDataType dataType, FString rowName)
{
	return GetDataTable<FTutorialType>(dataType, rowName);
}

UDataTable* UDataManager::GetSkillDataTable(ECharacterName characterName)
{
	FString dataType;
	switch (characterName)
	{
	case ECharacterName::Cardenio:
		dataType = UEnum::GetValueAsString(CardenioSkillData);
		break;
	case ECharacterName::Samira:
		dataType = UEnum::GetValueAsString(SamiraSkillData);
		break;
	case ECharacterName::Fernando:
		dataType = UEnum::GetValueAsString(FernandoSkillData);
		break;
	case ECharacterName::RuthCinda:
		dataType = UEnum::GetValueAsString(RuthCindaSkillData);
		break;
	default:
		return nullptr;
	}

	UDataTable** dataTable = _dataTableMap.Find(dataType);
	return *dataTable;
}

UDataTable* UDataManager::GetNPCSkillDataTable(const FString& characterName)
{
	FString npcSkillDatatTableName = characterName + NPCSkillDataTableName;
	UDataTable** dataTable = _dataTableMap.Find(npcSkillDatatTableName);
	return *dataTable;
}


FCharacterInfo* UDataManager::GetNPCCharacterInfo(FString& rowName)
{
	return GetDataTable<FCharacterInfo>(NPCData, rowName);
}


void UDataManager::AddDataTable(EDataType eName, UDataTable* dataTable)
{
	FString name = UEnum::GetValueAsString(eName);
	_dataTableMap.Add(name, dataTable);
}

void UDataManager::AddDataTable(const FString& name, UDataTable* dataTable)
{
	_dataTableMap.Add(name, dataTable);
}

void UDataManager::AddDataTable(EDataType eName, const FString& name)
{
	FString dataPath = FString::Printf(TEXT("%s/%s/%s.%s"), *DataTablePath, *UEnum::GetValueAsString(eName), *name,
	                                   *name);
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(
		*dataPath);

	if (DataTableFinder.Succeeded())
	{
		UDataTable* dataTable = Cast<UDataTable>(DataTableFinder.Object);
		AddDataTable(eName, dataTable);
	}
	else
	{
		LOG(Error, "%s Not Find DataTable", *dataPath);
	}
}


template <typename T>
T* UDataManager::GetDataTable(EDataType dataType, int rowNum)
{
	FString tableName = UEnum::GetValueAsString(dataType);
	UDataTable** dataTable = _dataTableMap.Find(tableName);

	if (dataTable && *dataTable)
	{
		return (*dataTable)->FindRow<T>(FName(*(FString::FormatAsNumber(rowNum))), TEXT("Find Data"));
	}
	return nullptr;
}

template <typename T>
T* UDataManager::GetDataTable(EDataType dataType, FString& rowName)
{
	FString tableName = UEnum::GetValueAsString(dataType);
	UDataTable** dataTable = _dataTableMap.Find(tableName);

	if (dataTable && *dataTable)
	{
		return (*dataTable)->FindRow<T>(FName(*rowName), TEXT("Find Data"));
	}
	return nullptr;
}
