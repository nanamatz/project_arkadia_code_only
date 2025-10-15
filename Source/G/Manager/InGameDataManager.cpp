// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Manager/InGameDataManager.h"

#include "DataManager.h"
#include "TutorialManager.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Save/PlayerCharacterInfoSaveGame.h"
#include "G/PlayerCharacter.h"
#include "G/Dialog/InteractionCharacter.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillTree.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "G/Dialog/FQuestManager.h"
#include "Kismet/GameplayStatics.h"


void UInGameDataManager::SaveInventoryData()
{
}

void UInGameDataManager::LoadInventoryData()
{
}

UInGameDataManager::UInGameDataManager()
{
	ClearPlayerCharacterMap();
}

void UInGameDataManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
	_controlCharacter = nullptr;
	_controlCharacterLocation = FVector::ZeroVector;
	_playerCharacterMap.Reset();
	_spawnPlayerCharacterInfoMap.Reset();
	_playerCharacterCount = 0;
	_playerSkillSlotList.Reset();
}

void UInGameDataManager::ChangeLevel()
{
	_controlCharacter = nullptr;
	_playerCharacterMap.Reset();
	_playerCharacterCount = 0;
}

TArray<APlayerCharacter*> UInGameDataManager::GetPlayerCharacterList()
{
	TArray<APlayerCharacter*> playerCharacters;
	for (auto& playerCharacter : _playerCharacterMap)
	{
		playerCharacters.Add(playerCharacter.Value);
	}
	return playerCharacters;
}

APlayerCharacter* UInGameDataManager::GetPlayerCharacter(ECharacterName characterName)
{
	if (QueryCharacter(characterName))
	{
		return _playerCharacterMap[characterName];
	}
	return nullptr;
}


bool UInGameDataManager::GetSpawnPlayerCharacterInfo(FCharacterInfo& out, ECharacterName characterName)
{
	if (_spawnPlayerCharacterInfoMap.Contains(characterName))
	{
		out = _spawnPlayerCharacterInfoMap[characterName];
		return true;
	}
	return false;
}

APlayerCharacter* UInGameDataManager::GetPlayerCharacter(int idx)
{
	TArray<APlayerCharacter*> playerCharacters = GetPlayerCharacterList();

	if (playerCharacters.IsValidIndex(idx))
	{
		return playerCharacters[idx];
	}

	return nullptr;
}

APlayerCharacter* UInGameDataManager::GetControlCharacter()
{
	return _controlCharacter;
}

FVector UInGameDataManager::GetControlCharacterLoaction()
{
	if (_controlCharacter)
	{
		SetControlCharacterLoaction();
	}
	return _controlCharacterLocation;
}

FCharacterInfo& UInGameDataManager::GetPlayerCharacterInfo(ECharacterName characterName)
{
	APlayerCharacter* character = GetPlayerCharacter(characterName);

	if (character)
	{
		return character->GetCharacterInfo();
	}
	
	return character->GetCharacterInfo();
}

TArray<USkillSlot*> UInGameDataManager::GetPlayerSkillSlotList(ECharacterName characterName)
{
	TArray<USkillSlot*> SkillSlots;
	if(_playerSkillSlotList.Contains(characterName))
	{
		SkillSlots = _playerSkillSlotList[characterName].skillSlotList; 
	}
	return SkillSlots;
}

void UInGameDataManager::SetControlCharacter(APlayerCharacter* character)
{
	if (character)
	{
		if (_controlCharacter)
		{
			_controlCharacter->SetIsControl(false);
		}
		_controlCharacter = character;
		_controlCharacter->SetIsControl(true);
		SetControlCharacterLoaction();
	}
}

void UInGameDataManager::SetControlCharacter(ECharacterName characterName)
{
	APlayerCharacter* character = GetPlayerCharacter(characterName);
	if (character && !character->GetBDeath())
	{
		_controlCharacter = character;
	}
}

void UInGameDataManager::SetControlCharacterLoaction()
{
	if (_controlCharacter)
	{
		_controlCharacterLocation = _controlCharacter->GetActorLocation();
	}
}

bool UInGameDataManager::QueryCharacter(ECharacterName characterName)
{
	return _playerCharacterMap.Contains(characterName);
}

bool UInGameDataManager::QuerySpawnCharacterInfo(ECharacterName characterName)
{
	return _spawnPlayerCharacterInfoMap.Contains(characterName);
}

void UInGameDataManager::ClearPlayerCharacterMap()
{
	_playerCharacterMap.Empty();
}


void UInGameDataManager::AddPlayerCharacterMap(ECharacterName characterName, APlayerCharacter* newCharacterClass, bool bNewCharacter)
{
	// 없는 경우에만 추가
	if (!QueryCharacter(characterName))
	{
		UE_LOG(LogTemp, Log, TEXT("Add NewPlayerCharacterMap %s"), *UEnum::GetValueAsString(characterName));
		_playerCharacterCount++;
		_playerCharacterMap.Add(characterName, newCharacterClass);
		InitPlayerCharacter(characterName, newCharacterClass, bNewCharacter);
	}
}

void UInGameDataManager::SaveCharacterInfo()
{
	TArray<APlayerCharacter*> playerCharacters = GetPlayerCharacterList();
	for (auto character : playerCharacters)
	{
		if (!character)
		{
			return;
		}
		ECharacterName eName = character->GetCharacterInfoManager()->GetECharacterName();
		FCharacterInfo info = character->GetCharacterInfoManager()->GetCharacterInfo();
		SetSpawnPlayerCharacterInfoMap(eName, info);
	}
}

void UInGameDataManager::AddSpawnPlayerCharacterInfoMap(ECharacterName characterName, FCharacterInfo characterInfo)
{
	if (!_spawnPlayerCharacterInfoMap.Contains(characterName))
	{
		FCharacterInfo saveCharacterInfo = characterInfo;
		LOG(Log, "Save Character Info : %s", *saveCharacterInfo.Name.ToString());
		_spawnPlayerCharacterInfoMap.Add(characterName, saveCharacterInfo);
	}
}

void UInGameDataManager::SetSpawnPlayerCharacterInfoMap(ECharacterName characterName, FCharacterInfo characterInfo)
{
	FCharacterInfo saveCharacterInfo = characterInfo;

	if (_spawnPlayerCharacterInfoMap.Contains(characterName))
	{
		_spawnPlayerCharacterInfoMap[characterName] = saveCharacterInfo;
	}
	else
	{
		characterInfo.equipped_item_map.Empty();
		AddSpawnPlayerCharacterInfoMap(characterName, characterInfo);
	}
}

void UInGameDataManager::AddSkillSlot(ECharacterName characterName, USkillSlot* NewSlot)
{
	if (!_playerSkillSlotList.Contains(characterName))
	{
		_playerSkillSlotList.Add(characterName);
		_playerSkillSlotList[characterName].skillSlotList.Add(NewSlot);
	}
	else if(!_playerSkillSlotList[characterName].skillSlotList.Contains(NewSlot))
	{
		_playerSkillSlotList[characterName].skillSlotList.Add(NewSlot);
	}
}

void UInGameDataManager::RemoveSkillSlot(ECharacterName characterName, USkillSlot* NewSlot)
{
	if(_playerSkillSlotList.Contains(characterName) && _playerSkillSlotList[characterName].skillSlotList.Contains(NewSlot))
	{
		_playerSkillSlotList[characterName].skillSlotList.Remove(NewSlot);
	}
}

bool UInGameDataManager::AddPOIMap(ACharacter* character)
{
	AGCharacter* g_char = Cast<AGCharacter>(character);


	
	if (IsValid(g_char))
	{
		if(g_char->GetECharacterName() == ECharacterName::NonPlayerCharacter)
		{
			return true;
		}
		
		if (!poi_map.Contains(g_char->GetCharacterName().ToString()))
		{
			poi_map.Add(g_char->GetCharacterName().ToString(), character);

			return true;
		}
	}
	else
	{
		AInteractionCharacter* ia_char = Cast<AInteractionCharacter>(character);

		if (IsValid(ia_char))
		{
			if (!poi_map.Contains(ia_char->CharName))
			{
				poi_map.Add(ia_char->CharName, character);

				return true;
			}
		}
	}

	return false;
}

void UInGameDataManager::RemovePOIMap(FString character_name)
{
	poi_map.Remove(character_name);
}

void UInGameDataManager::ChangeCurrentLevel(EGameLevel loadedMap)
{
	LOG(Log , "Change Current Level : %s", *UEnum::GetDisplayValueAsText(loadedMap).ToString())
	_currentLevel = loadedMap;
}

void UInGameDataManager::SaveData(int saveIndex, FString saveName)
{
	UPlayerCharacterInfoSaveGame* SaveGameInstance = Cast<UPlayerCharacterInfoSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerCharacterInfoSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		SaveGameInstance->SaveIndex = saveIndex;
		SaveGameInstance->SaveSlotName = saveName + TEXT("/InGameDataSaveSlot");

		for (auto playerCharacter : _playerCharacterMap)
		{
			ECharacterName characterName = playerCharacter.Key;
			APlayerCharacter* playerCharacterActor = playerCharacter.Value;
			playerCharacterActor->GetCharacterInfoManager()->SetLocation(playerCharacterActor->GetActorLocation());
		
			SaveGameInstance->CharacterInfo.Add(characterName, playerCharacterActor->GetCharacterInfo());
			
			LOG(Log, "Save : %s, Location : %s", *UEnum::GetValueAsString(characterName), *SaveGameInstance->CharacterInfo[characterName].CharacterLocation.ToString());
			
			for(auto skillSlot : playerCharacterActor->GetSkillSystemComponent()->GetSkillSlotListRef())
			{
				if(!skillSlot) continue;
				LOG(Log, "Save Skill : %s", *skillSlot->GetSkillInfo().skillClass->GetName());
				SaveGameInstance->AddCharacterSkill(characterName,skillSlot);
			}
			
		}

		LOG(Log, "Save : %s", *UEnum::GetValueAsString(_currentLevel));
		SaveGameInstance->CurrentLevel =  _currentLevel;	

		GameInstance->QuestManager->SaveQuestManager(SaveGameInstance->QuestArray, SaveGameInstance->DoingQuestArray,
			SaveGameInstance->EndQuestArray, SaveGameInstance->QuestHashTagArray, SaveGameInstance->GoalTypeArray,
			SaveGameInstance->ProgressArray);

		for(auto tutorial : GameInstance->GetTutorialManager()->GetTutorialSet())
		{
			SaveGameInstance->TutorialSet.Add(tutorial);
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->SaveIndex);
	}
	
	LOG(Log, "SaveGameInstance error");
}

void UInGameDataManager::LoadData(int saveIndex, FString saveName)
{
	UPlayerCharacterInfoSaveGame* LoadGameInstance = Cast<UPlayerCharacterInfoSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerCharacterInfoSaveGame::StaticClass()));

	if (LoadGameInstance)
	{
		LoadGameInstance->SaveSlotName = saveName + TEXT("/InGameDataSaveSlot");
		LoadGameInstance->SaveIndex = saveIndex;

		LoadGameInstance = Cast<UPlayerCharacterInfoSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->SaveIndex));

		if(!LoadGameInstance)
			return;
			
		GameInstance->QuestManager->LoadQuestManager(LoadGameInstance->QuestArray, LoadGameInstance->DoingQuestArray,
			LoadGameInstance->EndQuestArray, LoadGameInstance->QuestHashTagArray, LoadGameInstance->GoalTypeArray,
			LoadGameInstance->ProgressArray);
			
		for (auto characterInfo : LoadGameInstance->CharacterInfo)
		{
			LOG(Log, "Load | name : %s, Location : %s", *UEnum::GetValueAsString(characterInfo.Key), *characterInfo.Value.CharacterLocation.ToString());
			SetSpawnPlayerCharacterInfoMap(characterInfo.Key, characterInfo.Value);
		}

		
		LOG(Log, "Load : %s", *UEnum::GetValueAsString(LoadGameInstance->CurrentLevel));
		ChangeCurrentLevel(LoadGameInstance->CurrentLevel);

		GameInstance->GetTutorialManager()->SetTutorialSet(LoadGameInstance->TutorialSet);
		
		_loadData = LoadGameInstance;
	}else
	{
		LOG(Log, "LoadGameInstance Null");
	}
}

void UInGameDataManager::InitPlayerCharacter(ECharacterName characterName, APlayerCharacter* character, bool bNewCharacter)
{
	FCharacterInfo info;
	if (GetSpawnPlayerCharacterInfo(info, characterName))
	{
		LOG(Log, "bIsStart : %d", GameInstance->GetIsStart());
		// isStart - false면 레벨 이동 or Load, true면 처음 시작
		if(bNewCharacter)
		{
			character->GetCharacterInfoManager()->SetCharacterInfo(info, bNewCharacter);
			character->GetSkillSystemComponent()->SetSkillSlot(
			character, GameInstance->GetDataManager()->GetSkillDataTable(characterName), bNewCharacter);
		}
		else
		{
			character->GetCharacterInfoManager()->SetCharacterInfo(info, GameInstance->GetIsStart());
			character->GetSkillSystemComponent()->SetSkillSlot(
			character, GameInstance->GetDataManager()->GetSkillDataTable(characterName), GameInstance->GetIsStart());
		}
		character->CreateBoundWeapon();
	}
	else
	{
		LOG(Error, "Not Find Character Data");
	}

}
