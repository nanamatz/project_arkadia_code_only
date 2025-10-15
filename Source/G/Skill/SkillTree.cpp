// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SkillTree.h"
#include "Engine/DataTable.h"
#include <G/Skill/SkillBase.h>
#include <G/Skill/SkillSlot.h>
#include <G/PlayerCharacter.h>
#include <G/Skill/SkillData/SkillStruct.h>

#include "SkillTreeNode.h"
#include "G/G.h"
#include "G/Manager/InGameDataManager.h"
#include "Kismet/GameplayStatics.h"

void USkillTree::SetSkillTree(AGCharacter* myCharacter, UDataTable* _skillTreeData, USkillSlot* mySkillSlot)
{
	GenerateSkillTree(myCharacter, mySkillSlot, _skillTreeData);
}

void USkillTree::SetSkillTree(AGCharacter* myCharacter, const TArray<struct FSkillTreeNodeStruct>& skillTreeNodeList, USkillSlot* mySkillSlot)
{
	GenerateSkillTree(myCharacter, mySkillSlot, skillTreeNodeList);
}


void USkillTree::SetPlayerCharacter(AGCharacter* PlayerCharacter)
{
	for (auto node : _skillTreeNodeList)
	{
		node->SetMyCharacter(PlayerCharacter);
	}
}

TArray<USkillTreeNode*> USkillTree::GetAllSkillTreeNodeList()
{
	//UE_LOG(LogTemp, Warning, TEXT("GetAllSkillTreeNodeList : %d"), skillTreeNodeList.Num());
	return _skillTreeNodeList;
}

TArray<FSkillTreeNodeStruct> USkillTree::GetAllSkillTreeNodeInfoList()
{
	TArray<FSkillTreeNodeStruct> SkillTreeNodeStructList;
	for(auto skillTreeNode : _skillTreeNodeList)
	{
		SkillTreeNodeStructList.Add(skillTreeNode->GetNodeInfo());
	}
	return SkillTreeNodeStructList;
}

void USkillTree::GenerateSkillTree(AGCharacter* myCharacter, USkillSlot* mySkillSlot, UDataTable* _skillTreeData)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));

	if (_skillTreeData)
	{
		TArray<FName> RowNames = _skillTreeData->GetRowNames();
		// 루트 만들기
		FSkillTreeNodeStruct* RootData = _skillTreeData->FindRow<FSkillTreeNodeStruct>(
			FName(*(FString::FormatAsNumber(0))), TEXT("Finding Tree Node Data"));
		_skillTreeRoot = NewObject<USkillTreeNode>(this);

		_skillTreeRoot->SetNodeInfo(*RootData, mySkillSlot->GetMySkill(), myCharacter);
		AddSkillTreeNodeList(_skillTreeRoot, mySkillSlot);

		for (int i = 1; i < RowNames.Num(); i++)
		{
			FSkillTreeNodeStruct* newNodeData = _skillTreeData->FindRow<FSkillTreeNodeStruct>(
				FName(*(FString::FormatAsNumber(i))), TEXT("Finding Tree Node Data"));
			USkillTreeNode* newNode = NewObject<USkillTreeNode>(this);

			newNode->SetNodeInfo(*newNodeData, mySkillSlot->GetMySkill(), myCharacter);
			// 스킬 오브젝트 생성
			AddSkillTreeNodeList(newNode, mySkillSlot);

			if (newNode->GetSkillTreeNodeRequirement().Num() == 0)
			{
				USkillTreeNode* parentNode = FindSkillTreeNode(newNode->GetTier() - 1, 0);
				if (parentNode)
				{
					parentNode->AddNodeList(newNode);
				}
			}
			for (int RequirementNum : newNode->GetSkillTreeNodeRequirement())
			{
				USkillTreeNode* ParentNode = FindSkillTreeNode(newNode->GetTier() - 1, RequirementNum);
				if (ParentNode)
				{
					ParentNode->AddNodeList(newNode);
				}
			}
		}

		//mySkillSlot->ChangeSkill(_skillTreeRoot->GetMySkill());
		for(auto skillTreeNode : _skillTreeNodeList)
		{
			LOG(Log, "%s : %d", *skillTreeNode->GetNodeName().ToString(), skillTreeNode->GetIsLearning());
			if(skillTreeNode->GetIsLearning())
			{
				skillTreeNode->ActiveNode();
			}
		}

		GetAllSkillTreeNodeList();
	}
	else
	{
	}
}

void USkillTree::GenerateSkillTree(AGCharacter* myCharacter, USkillSlot* mySkillSlot, const TArray<struct FSkillTreeNodeStruct>& skillTreeNodeList)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));

	if(skillTreeNodeList.Num() <= 0) return;
	// 루트 만들기
	FSkillTreeNodeStruct* RootData = new FSkillTreeNodeStruct(skillTreeNodeList[0]); 
	_skillTreeRoot = NewObject<USkillTreeNode>(this);

	_skillTreeRoot->SetNodeInfo(*RootData, mySkillSlot->GetMySkill(), myCharacter);
	AddSkillTreeNodeList(_skillTreeRoot, mySkillSlot);

	bool rootInit = false;
	for (auto& skillTreeNode : skillTreeNodeList)
	{
		if(!rootInit)
		{
			rootInit = true;
			continue;
		}
		FSkillTreeNodeStruct* newNodeData = new FSkillTreeNodeStruct(skillTreeNode);
		USkillTreeNode* newNode = NewObject<USkillTreeNode>(this);

		newNode->SetNodeInfo(*newNodeData, mySkillSlot->GetMySkill(), myCharacter);
		// 스킬 오브젝트 생성
		AddSkillTreeNodeList(newNode, mySkillSlot);

		if (newNode->GetSkillTreeNodeRequirement().Num() == 0)
		{
			USkillTreeNode* parentNode = FindSkillTreeNode(newNode->GetTier() - 1, 0);
			if (parentNode)
			{
				parentNode->AddNodeList(newNode);
			}
		}
		for (int RequirementNum : newNode->GetSkillTreeNodeRequirement())
		{
			USkillTreeNode* ParentNode = FindSkillTreeNode(newNode->GetTier() - 1, RequirementNum);
			if (ParentNode)
			{
				ParentNode->AddNodeList(newNode);
			}
		}
	}
	
	//mySkillSlot->ChangeSkill(_skillTreeRoot->GetMySkill());
	for(auto skillTreeNode : _skillTreeNodeList)
	{
		LOG(Log, "%s : %d", *skillTreeNode->GetNodeName().ToString(), skillTreeNode->GetIsLearning());
		if(skillTreeNode->GetIsLearning())
		{
			skillTreeNode->ActiveNode();
		}
	}

	GetAllSkillTreeNodeList();

}

void USkillTree::AddSkillTreeNodeList(USkillTreeNode* newNode, USkillSlot* mySkillSlot)
{
	//newNode->GetMySkill()->SetSkill(mySkillSlot);
	_skillTreeNodeList.Add(newNode);
}

USkillTreeNode* USkillTree::FindSkillTreeNode(int _tierNum, int _nodeNum)
{
	if (!_skillTreeRoot)
	{
		return nullptr;
	}

	// 스택 대신 큐를 사용하여 너비 우선 탐색(BFS)을 수행합니다.
	TQueue<USkillTreeNode*> NodeQueue;
	NodeQueue.Enqueue(_skillTreeRoot);


	while (!NodeQueue.IsEmpty())
	{
		USkillTreeNode* CurrentNode;
		NodeQueue.Dequeue(CurrentNode);

		// 현재 노드의 값을 출력합니다.
		if (CurrentNode->GetTier() == _tierNum && CurrentNode->GetNum() == _nodeNum)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Find Node : %s"), *CurrentNode->NodeName.ToString());
			return CurrentNode;
		}

		// 현재 노드의 모든 자식 노드를 큐에 추가합니다.
		for (USkillTreeNode* Child : CurrentNode->GetChildNodeList())
		{
			NodeQueue.Enqueue(Child);
		}
	}
	return nullptr;
}

class USkillBase* USkillTree::GetSkillClassObject(int _tierNum, int _nodeNum)
{
	USkillTreeNode* node = FindSkillTreeNode(_tierNum, _nodeNum);
	if (node)
	{
		return node->GetMySkill();
	}
	return nullptr;
}

USkillTreeNode* USkillTree::FindSkillTreeNode(FString _nodeName)
{
	if (!_skillTreeRoot)
	{
		return nullptr;
	}

	// 스택 대신 큐를 사용하여 너비 우선 탐색(BFS)을 수행합니다.
	TQueue<USkillTreeNode*> NodeQueue;
	NodeQueue.Enqueue(_skillTreeRoot);


	while (!NodeQueue.IsEmpty())
	{
		USkillTreeNode* CurrentNode;
		NodeQueue.Dequeue(CurrentNode);

		// 현재 노드의 값을 출력합니다.
		if (CurrentNode->GetNodeName().ToString().Equals(_nodeName))
		{
			return CurrentNode;
		}

		// 현재 노드의 모든 자식 노드를 큐에 추가합니다.
		for (USkillTreeNode* Child : CurrentNode->GetChildNodeList())
		{
			NodeQueue.Enqueue(Child);
		}
	}
	return nullptr;
}

class USkillBase* USkillTree::GetSkillClassObject(FString _nodeName)
{
	USkillTreeNode* node = FindSkillTreeNode(_nodeName);
	if (node)
	{
		return node->GetMySkill();
	}
	return nullptr;
}
