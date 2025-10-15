// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Manager/ManagerBase.h"
#include <G/Util/Define.h>
#include "G/Character/CharacterInfo.h"
#include "G/Skill/SkillSlot.h"
#include "InGameDataManager.generated.h"

class AGCharacter;

USTRUCT()
struct FPlayerSkillSlot
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<class USkillSlot*> skillSlotList;
};

/**
 * 
 */
UCLASS()
class G_API UInGameDataManager : public UManagerBase
{
	GENERATED_BODY()

	int _playerCharacterCount = 0;

	// 현재 맵에 활동 중인 캐릭터 클래스
	UPROPERTY()
	TMap<ECharacterName, class APlayerCharacter*> _playerCharacterMap;

	// 게임에서 생성된 플레이어 캐릭터 데이터 정보를 저장할 Map, 지금은 생성 확인 용도로만 사용되고 있음.
	// Map에 있는 데이터를 사용해서 맵이 바뀔 때 캐릭터 정보 업데이트 X 
	UPROPERTY()
	TMap<ECharacterName, struct FCharacterInfo> _spawnPlayerCharacterInfoMap;
	
	UPROPERTY()
	TMap<ECharacterName, FPlayerSkillSlot> _playerSkillSlotList;
	
	UPROPERTY()
	EGameLevel _currentLevel;
	
	UPROPERTY()
	class APlayerCharacter* _controlCharacter;
	FVector _controlCharacterLocation;

	UPROPERTY()
	TMap<FString, ACharacter*> poi_map;
	
private:
	void SaveInventoryData();
	void LoadInventoryData();
	
public:

	UPROPERTY()
	class UPlayerCharacterInfoSaveGame* _loadData;
	
	UInGameDataManager();

	virtual void Init(class UGGameInstance* gameInstance) override;
	void ChangeLevel();
	
	UFUNCTION(BlueprintCallable)
	TArray<class APlayerCharacter*> GetPlayerCharacterList();
	class APlayerCharacter* GetPlayerCharacter(ECharacterName characterName);

	TMap<ECharacterName, struct FCharacterInfo>& GetSpawnPlayerCharacterInfoMap()
	{
		return _spawnPlayerCharacterInfoMap;
	}

	bool GetSpawnPlayerCharacterInfo(struct FCharacterInfo& out, ECharacterName characterName);
	class APlayerCharacter* GetPlayerCharacter(int idx);
	UFUNCTION(BlueprintCallable)
	class APlayerCharacter* GetControlCharacter();
	FVector GetControlCharacterLoaction();
	struct FCharacterInfo& GetPlayerCharacterInfo(ECharacterName characterName);
	int GetPlayerCharacterCount() { return _playerCharacterCount; }
	EGameLevel GetCurrentLevel() const { return _currentLevel; }
	TArray<USkillSlot*> GetPlayerSkillSlotList(ECharacterName characterName);

	
	void SetControlCharacter(class APlayerCharacter* character);
	void SetControlCharacter(ECharacterName characterName);
	void SetControlCharacterLoaction();

	bool QueryCharacter(ECharacterName characterName);
	bool QuerySpawnCharacterInfo(ECharacterName characterName);
	void ClearPlayerCharacterMap();

	void InitPlayerCharacter(ECharacterName characterName, class APlayerCharacter* character, bool bNewCharacter = false);
	void AddPlayerCharacterMap(ECharacterName characterName, class APlayerCharacter* newCharacterClass, bool bNewCharacter = false);

	void SaveCharacterInfo();
	void AddSpawnPlayerCharacterInfoMap(ECharacterName characterName, FCharacterInfo characterInfo);
	void SetSpawnPlayerCharacterInfoMap(ECharacterName characterName, FCharacterInfo characterInfo);

	void AddSkillSlot(ECharacterName characterName, class USkillSlot* NewSlot);
	void RemoveSkillSlot(ECharacterName characterName, USkillSlot* NewSlot);

	bool AddPOIMap(ACharacter* character);
	void RemovePOIMap(FString character_name);

	void ChangeCurrentLevel(EGameLevel loadedMap);

	void SaveData(int saveIndex = 0, FString saveName = "Default");
	void LoadData(int saveIndex = 0, FString saveName = "Default");
};
