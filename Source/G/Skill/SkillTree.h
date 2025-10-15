// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <G/Skill/SkillTreeNodeStruct.h>

#include "G/PlayerCharacter.h"
#include "SkillTree.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillTree : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class USkillTreeNode* _skillTreeRoot;

	UPROPERTY()
	TArray<class USkillTreeNode*> _skillTreeNodeList;

public:
	class USkillTreeNode* GetSkillTreeRoot()
	{
		return _skillTreeRoot;
	}

	UFUNCTION()
	void SetSkillTree(class AGCharacter* myCharacter, class UDataTable* _skillTreeData, class USkillSlot* mySkillSlot);
	void SetSkillTree(AGCharacter* myCharacter, const TArray<struct FSkillTreeNodeStruct>& skillTreeNodeList, USkillSlot* mySkillSlot);

	void SetPlayerCharacter(class AGCharacter* PlayerCharacter);

	TArray<class USkillTreeNode*> GetAllSkillTreeNodeList();
	TArray<struct FSkillTreeNodeStruct> GetAllSkillTreeNodeInfoList();
	
	UFUNCTION()
	void GenerateSkillTree(class AGCharacter* myCharacter, class USkillSlot* mySkillSlot,
	                       class UDataTable* _skillTreeData);
	void GenerateSkillTree(AGCharacter* myCharacter, USkillSlot* mySkillSlot,
							const TArray<struct FSkillTreeNodeStruct>& skillTreeNodeList);
	void AddSkillTreeNodeList(class USkillTreeNode* newNode, class USkillSlot* mySkillSlot);

	class USkillTreeNode* FindSkillTreeNode(FString _nodeName);
	class USkillTreeNode* FindSkillTreeNode(int _tierNum, int NodeNum);
	class USkillBase* GetSkillClassObject(FString _nodeName);
	class USkillBase* GetSkillClassObject(int _tierNum, int _nodeNum);
};
