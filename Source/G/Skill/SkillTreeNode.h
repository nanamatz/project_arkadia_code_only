// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillTreeNodeStruct.h"
#include "UObject/NoExportTypes.h"
#include "SkillTreeNode.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillTreeNode : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FSkillTreeNodeStruct _nodeInfo;

	UPROPERTY()
	class AGCharacter* _myCharacter;

	UPROPERTY()
	class USkillBase* _mySkill;

	// 연결된 노드 리스트, SkillTreeNodeRequirement를 보고 부모 연결될 노드를 선택
	UPROPERTY()
	TArray<USkillTreeNode*> _childNodeList;


public:
	struct FSkillTreeNodeStruct& GetNodeInfo() { return _nodeInfo; }
	int GetNum() { return _nodeInfo.Num; }
	int GetTier() { return _nodeInfo.Tier; }
	FText GetNodeName() { return _nodeInfo.NodeName; }
	FText GetDescription() { return _nodeInfo.Description; }
	class UTexture2D* GetThumbnail() { return _nodeInfo.Thumbnail; }
	TArray<int> GetSkillTreeNodeRequirement() { return _nodeInfo.SkillTreeNodeRequirement; }
	TArray<bool> GetIsRequirementMet() { return _nodeInfo.isRequirementMet; }
	class USkillBase* GetMySkill() { return _mySkill; }
	TArray<class USkillTreeNode*> GetChildNodeList() { return _childNodeList; }
	FAbility& GetAbilityRequirement() { return _nodeInfo.AbilityRequirement; }
	int32 GetSkillPointRequirement() const { return _nodeInfo.SkillPointRequirement; }
	bool GetIsLearning() { return _nodeInfo.IsLearning; }

	void SetMyCharacter(class AGCharacter* myCharacter);
	void SetNodeInfo(struct FSkillTreeNodeStruct nodeInfo, USkillBase* mySkill, class AGCharacter* myCharacter);
	void SetIsRequirementMet(int num, bool value);
	void SetIsLearning(bool value) { _nodeInfo.IsLearning = value; }

	void AddNodeList(class USkillTreeNode* node) { _childNodeList.Add(node); }

	void ActiveNode();
	virtual void ClickTreeNode();

	bool CheckLearning();
};
