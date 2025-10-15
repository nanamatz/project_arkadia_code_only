// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTreeNodeButton.h"

#include "SkillTreeNodeTooltip.h"
#include "G/G.h"
#include "G/UI/Skill/SkillCompanionToggleButton.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillTree.h"
#include "G/Skill/SkillTreeNode.h"

void USkillTreeNodeButton::Init(USkillTreeNode* node, USkillTreeNodeTooltip* _tooltip)
{
	skillTreeNode = node;
	tooltip = _tooltip;

	this->WidgetStyle.Normal.SetImageSize(FVector2D(100, 100));
	this->WidgetStyle.Normal.SetResourceObject(skillTreeNode->GetThumbnail());
	this->WidgetStyle.Normal.Margin = 0;

	this->WidgetStyle.Hovered.SetImageSize(FVector2D(100, 100));
	this->WidgetStyle.Hovered.SetResourceObject(skillTreeNode->GetThumbnail());
	this->WidgetStyle.Hovered.Margin = 0;

	this->WidgetStyle.Pressed.SetImageSize(FVector2D(100, 100));
	this->WidgetStyle.Pressed.SetResourceObject(skillTreeNode->GetThumbnail());
	this->WidgetStyle.Pressed.Margin = 0;

	if (skillTreeNode->GetIsLearning())
	{
		this->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 1));
		this->WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 1));
		this->WidgetStyle.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 1));
	}
	else
	{
		this->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0.2));
		this->WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0.2));
		this->WidgetStyle.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0.2));
	}


	OnClicked.AddDynamic(this, &USkillTreeNodeButton::OnStartButtonClicked);
	OnHovered.AddDynamic(this, &USkillTreeNodeButton::OnTooltip);
	OnUnhovered.AddDynamic(this, &USkillTreeNodeButton::OffTooltip);
}

bool USkillTreeNodeButton::LearnSkillNode()
{
	return skillTreeNode->CheckLearning();
}

FSkillTreeNodeStruct& USkillTreeNodeButton::GetNodeInfo()
{
	return skillTreeNode->GetNodeInfo();
}

void USkillTreeNodeButton::OnStartButtonClicked()
{
	if (skillTreeNode->GetIsLearning())
	{
		LOG(Log, "Is Learning True");
		return;
	}
	if (LearnSkillNode())
	{
		skillTreeNode->SetIsLearning(true);
		skillTreeNode->ClickTreeNode();

		for (auto node : skillTreeNode->GetChildNodeList())
		{
			node->SetIsRequirementMet(skillTreeNode->GetNum(), true);
		}

		this->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 1));
		this->WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 1));
		this->WidgetStyle.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 1));
	}
}

void USkillTreeNodeButton::OnTooltip()
{
	tooltip->On(skillTreeNode);
}

void USkillTreeNodeButton::OffTooltip()
{
	tooltip->Off();
}
