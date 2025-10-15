// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Skill/SkillTreeNodeTooltip.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "G/Skill/SkillTreeNode.h"
#include "G/Skill/SkillTreeNodeStruct.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void USkillTreeNodeTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D viewport_center = UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / 2.0f;
	FVector2D mouse_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	if(mouse_pos.X > viewport_center.X)
	{
		mouse_pos.X -= GetDesiredSize().X + 10;
	} else
	{
		mouse_pos.X += 20;
	}

	if(mouse_pos.Y > viewport_center.Y)
	{
		mouse_pos.Y -= GetDesiredSize().Y + 10;
	} else
	{
		mouse_pos.Y += 10;
	}

	SetPositionInViewport(mouse_pos, false);
}

void USkillTreeNodeTooltip::On(USkillTreeNode* node)
{
	bHasScriptImplementedTick = true;

	FVector2D viewport_center = UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / 2.0f;
	FVector2D mouse_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	if(mouse_pos.X > viewport_center.X)
	{
		mouse_pos.X -= GetDesiredSize().X + 10;
	} else
	{
		mouse_pos.X += 20;
	}

	if(mouse_pos.Y > viewport_center.Y)
	{
		mouse_pos.Y -= GetDesiredSize().Y + 10;
	} else
	{
		mouse_pos.Y += 10;
	}

	SetPositionInViewport(mouse_pos, false);
	
	FString requirement_str;

	for (USkillTreeNode* requirement_node : node->GetChildNodeList())
	{
		requirement_str += requirement_node->GetNodeName().ToString() + " ";
	}

	skill_name->SetText(
		FText::Format(LOCTEXT("SkillTreeTooltipSkillName", "{0}"), node->GetNodeName()));
	description->SetText(
		FText::Format(LOCTEXT("SkillTreeTooltipDesc", "{0}"), node->GetDescription()));
	requirement_text->SetText(
		FText::Format(LOCTEXT("SkillTreeNodeTooltipRequirementText", "{0}"), FText::FromString(requirement_str)));

	AddToViewport();
}

void USkillTreeNodeTooltip::Off()
{
	bHasScriptImplementedTick = false;

	RemoveFromViewport();
}

#undef LOCTEXT_NAMESPACE
