// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Skill/SkillTreeUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "SkillTreeNodeButton.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "G/Skill/SkillTreeNode.h"

class UVerticalBoxSlot;
class USkillTreeNode;
class USkillTreeNodeButton;

int32 USkillTreeUI::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                int32 LayerId,
                                const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 NewLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId,
	                                      InWidgetStyle, bParentEnabled);

	FLinearColor LineColor = FLinearColor::White;
	LineColor.A = 0.8f;

	for (int col = 1; col < skill_tree->GetChildrenCount(); col++)
	{
		UHorizontalBox* upper_node_box = Cast<UHorizontalBox>(skill_tree->GetChildAt(col - 1));
		UHorizontalBox* lower_node_box = Cast<UHorizontalBox>(skill_tree->GetChildAt(col));

		for (auto _node : lower_node_box->GetAllChildren())
		{
			USkillTreeNodeButton* lower_node = Cast<USkillTreeNodeButton>(_node);

			FGeometry lower_node_geometry = lower_node->GetCachedGeometry();

			for (int row : lower_node->GetNodeInfo().SkillTreeNodeRequirement)
			{
				USkillTreeNodeButton* upper_node = Cast<USkillTreeNodeButton>(upper_node_box->GetChildAt(row));

				FVector2D StartPoint = skill_tree->GetCachedGeometry().AbsoluteToLocal(
						lower_node->GetCachedGeometry().AbsolutePosition) + lower_node->GetCachedGeometry().
					GetLocalSize() /
					2;
				StartPoint.Y -= lower_node->GetCachedGeometry().GetLocalSize().Y / 2;

				FVector2D EndPoint = skill_tree->GetCachedGeometry().AbsoluteToLocal(
						upper_node->GetCachedGeometry().AbsolutePosition) + upper_node->GetCachedGeometry().
					GetLocalSize() /
					2;
				EndPoint.Y += upper_node->GetCachedGeometry().GetLocalSize().Y / 2;

				FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
				                      bParentEnabled);

				UWidgetBlueprintLibrary::DrawLines(Context, {EndPoint, StartPoint}, LineColor, true, 2.0f);
			}
		}
	}

	return NewLayerId;
}

/*
* Illustration of the initial skill tree UI structure
* May need to add a group option in the future. (idea)
*/
void USkillTreeUI::AddNode(USkillTreeNode* node, int tree_idx, USkillTreeNodeTooltip* _tooltip)
{
	if (node->GetNum() == 0)
	{
		UVerticalBoxSlot* vb_slot = skill_tree->AddChildToVerticalBox(
			WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass()));
		vb_slot->SetSize(ESlateSizeRule::Fill);
	}

	UHorizontalBox* hb = Cast<UHorizontalBox>(skill_tree->GetChildAt(node->GetTier() - 1));
	USkillTreeNodeButton* bt = WidgetTree->ConstructWidget<USkillTreeNodeButton>(USkillTreeNodeButton::StaticClass());

	bt->Init(node, _tooltip);

	UHorizontalBoxSlot* hb_slot = hb->AddChildToHorizontalBox(bt);
	hb_slot->SetSize(ESlateSizeRule::Fill);
	hb_slot->SetHorizontalAlignment(HAlign_Center);
	hb_slot->SetVerticalAlignment(VAlign_Center);
}
