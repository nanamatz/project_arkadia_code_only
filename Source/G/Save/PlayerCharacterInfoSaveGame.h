// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Character/CharacterInfo.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillTree.h"
#include "GameFramework/SaveGame.h"
#include "G/Skill/SkillTreeNodeStruct.h"
#include "PlayerCharacterInfoSaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct FQuestInfoSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	FString Content;
	UPROPERTY(EditAnywhere)
	int32 State;
	UPROPERTY(EditAnywhere)
	bool isDo;
	UPROPERTY(EditAnywhere)
	bool isCompletion;
	UPROPERTY(EditAnywhere)
	int32 ArrayIndex;
	UPROPERTY(EditAnywhere)
	TArray<bool> isVisibles;

	UPROPERTY()
	TMap<FString, int32> KillEnemyList;

	UPROPERTY()
	TArray<int32> EnemyKillCount;

	UPROPERTY()
	TMap<FString, int32> RequiredItemsList;

	UPROPERTY()
	TArray<class AInteractionCharacter*> AssignedCharacters;

	UPROPERTY()
	bool isAutomationSet;

	UPROPERTY()
	TMap<FSoftObjectPath, int32> CompleteRewardItemsList;

	UPROPERTY()
	int32 CompleteRewardGold;

	UPROPERTY()
	int32 CompleteRewardExp;

	UPROPERTY()
	int32 CompleteSetState;
};

USTRUCT(BlueprintType)
struct FSkillSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkillInfo")
	TSubclassOf<USkillBase> skill;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkillInfo")
	FSkillStruct skillInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkillInfo")
	TArray<FSkillTreeNodeStruct> skillTree;
};

USTRUCT(BlueprintType)
struct FSkillSaveDataList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSkillInfo")
	TArray<FSkillSaveData> skillList;

	void AddSkillSaveData(TSubclassOf<USkillBase> skill, FSkillStruct skillInfo, TArray<FSkillTreeNodeStruct> skillTree)
	{
		skillList.Add({skill, skillInfo, skillTree});
	}

	void AddSkillSaveData(USkillSlot* skillSlot)
	{
		TArray<FSkillTreeNodeStruct> skillTreeNodeStructList;
		if(skillSlot->GetMySkillTree())
		{
			skillTreeNodeStructList = skillSlot->GetMySkillTree()->GetAllSkillTreeNodeInfoList();
		}
		skillList.Add({skillSlot->GetSkillInfo().skillClass, skillSlot->GetSkillInfo(), skillTreeNodeStructList});
	}


	FSkillSaveData GetSkillSaveData(int idx)
	{
		return skillList[idx];
	}
};

UCLASS()
class G_API UPlayerCharacterInfoSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FString SaveSlotName;		// Save game data file name

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 SaveIndex;		// Save game data file index

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterInfo")
	TMap<ECharacterName, FCharacterInfo> CharacterInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MapInfo")
	EGameLevel CurrentLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterSkillInfo")
	TMap<ECharacterName, FSkillSaveDataList> CharacterSkill;

	UPROPERTY()
	TArray<FQuestInfoSaveData> QuestArray;

	UPROPERTY()
	TArray<FQuestInfoSaveData> DoingQuestArray;

	UPROPERTY()
	TArray<FQuestInfoSaveData> EndQuestArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QuestHashTagInfo")
	TArray<FString> QuestHashTagArray;

	UPROPERTY()
	TArray<int32> GoalTypeArray;

	UPROPERTY()
	TArray<int32> ProgressArray;

	UPROPERTY()
	TSet<ETutorial> TutorialSet; 
	
	void AddCharacterSkill(ECharacterName characterName, class USkillSlot* skillSlot);
	//void AddLoadMap();
};
