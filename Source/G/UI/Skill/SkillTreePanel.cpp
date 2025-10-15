// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Skill/SkillTreePanel.h"

#include "SkillTreeNodeButton.h"
#include "SkillTreeUI.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillTree.h"
#include "G/PlayerCharacter.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "G/Skill/SkillTreeNode.h"

class USkillTreeNodeButton;
class USkillTreeNode;
class UHorizontalBoxSlot;
class USkillTree;

void USkillTreePanel::Init(UWidgetTree* _WidgetTree, APlayerCharacter* character, USkillTreeNodeTooltip* _tooltip)
{
	this->SetVisibility(ESlateVisibility::Hidden);

	int idx = -1;

	int skillAmount = character->GetSkillSystemComponent()->GetSkillSlotList().Num();

	// skill_trees = new UVerticalBox*[skillAmount - 1];

	for (USkillSlot* skillSlot : character->GetSkillSystemComponent()->GetSkillSlotList())
	{
		LOG(Log, "Init SkillTreePanel : %s", *skillSlot->GetMySkill()->GetSkillInfoManager()->GetSkillName().ToString())
		// 첫 번째 기본공격 스킬트리는 넘어가기
		if (skillSlot && skillSlot->GetMySkillTree() && skillSlot->GetMySkillTree()->GetAllSkillTreeNodeList().Num() <= 1)
		{
			idx++;
			continue;
		}
		USkillTree* mySkillTree = skillSlot->GetMySkillTree();
		// 스킬트리가 없으면 넘기기 ( 기본공격 )
		TArray<USkillTreeNode*> mySkillTreeNodeList = mySkillTree->GetAllSkillTreeNodeList();


		USkillTreeUI* new_skill_tree_ui = CreateWidget<USkillTreeUI>(this, bp_skill_tree_ui);

		skill_tree_ui_arr.Add(new_skill_tree_ui);

		UHorizontalBoxSlot* hb_slot = skill_tree->AddChildToHorizontalBox(new_skill_tree_ui);

		hb_slot->SetSize(ESlateSizeRule::Fill);

		USkillTree* skillTree = skillSlot->GetMySkillTree();

		for (int i = 0; i < mySkillTreeNodeList.Num(); i++)
		{
			new_skill_tree_ui->AddNode(skillTree->FindSkillTreeNode(mySkillTreeNodeList[i]->GetNodeName().ToString()), idx,
			                           _tooltip);
		}

		idx++;
	}
}

void USkillTreePanel::CreateSkillTree(USkillTreeNode* node, int tree_idx, USkillTreeNodeTooltip* _tooltip)
{
	if (node->GetNum() == 0)
	{
		UVerticalBoxSlot* vb_slot = skill_tree_ui_arr[tree_idx]->skill_tree->AddChildToVerticalBox(
			WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass()));
		vb_slot->SetSize(ESlateSizeRule::Fill);
	}

	UHorizontalBox* hb = Cast<UHorizontalBox>(skill_tree_ui_arr[tree_idx]->skill_tree->GetChildAt(node->GetTier() - 1));
	USkillTreeNodeButton* bt = WidgetTree->ConstructWidget<USkillTreeNodeButton>(USkillTreeNode::StaticClass());

	bt->Init(node, _tooltip);

	UHorizontalBoxSlot* hb_slot = hb->AddChildToHorizontalBox(bt);
	hb_slot->SetSize(ESlateSizeRule::Fill);
	hb_slot->SetHorizontalAlignment(HAlign_Center);
	hb_slot->SetVerticalAlignment(VAlign_Center);
}
