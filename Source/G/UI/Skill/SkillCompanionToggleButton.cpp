// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Skill/SkillCompanionToggleButton.h"

#include "SkillTreePanel.h"
#include "SkillTreeUI.h"
#include "G/PlayerCharacter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void USkillCompanionToggleButton::ToggleSkillTreeUI()
{
	(*current_skill_tree_panel)->SetVisibility(ESlateVisibility::Hidden);

	skill_tree_panel->SetVisibility(ESlateVisibility::Visible);

	(*current_skill_tree_panel) = skill_tree_panel;
}

/*
* Init Companion Toggle Button
*/
void USkillCompanionToggleButton::Init(UOverlay* skill_tree_overlay, USkillTreePanel** _current_skill_tree_panel,
                                       APlayerCharacter* character, USkillTreeNodeTooltip* _tooltip)
{
	
	target_character_name = character->GetECharacterName();

	current_skill_tree_panel = _current_skill_tree_panel;

	if (*_current_skill_tree_panel == nullptr)
	{
		*current_skill_tree_panel = skill_tree_panel;
	}

	this->order = character->GetCharacterNum();

	/* Create skill tree ui */
	skill_tree_panel = CreateWidget<USkillTreePanel>(this, bp_skill_tree_panel);

	UOverlaySlot* overlay_slot = skill_tree_overlay->AddChildToOverlay(skill_tree_panel);
	overlay_slot->SetHorizontalAlignment(HAlign_Fill);
	overlay_slot->SetVerticalAlignment(VAlign_Fill);

	skill_tree_panel->Init(WidgetTree, character, _tooltip);

	/* Set Slot details */
	UVerticalBoxSlot* vertical_box_slot = Cast<UVerticalBoxSlot>(this->Slot);
	vertical_box_slot->SetSize(ESlateSizeRule::Fill);
	vertical_box_slot->SetHorizontalAlignment(HAlign_Center);
	vertical_box_slot->SetVerticalAlignment(VAlign_Center);

	/* Set Button Style */
	button->WidgetStyle.Normal.SetResourceObject(&character->GetPortrait());
	button->WidgetStyle.Normal.SetImageSize(FVector2D(100, 100));
	button->WidgetStyle.Normal.Margin = 0;

	button->WidgetStyle.Hovered.SetResourceObject(&character->GetPortrait());
	button->WidgetStyle.Hovered.SetImageSize(FVector2D(100, 100));
	button->WidgetStyle.Hovered.Margin = 0;

	button->WidgetStyle.Pressed.SetResourceObject(&character->GetPortrait());
	button->WidgetStyle.Pressed.SetImageSize(FVector2D(100, 100));
	button->WidgetStyle.Pressed.Margin = 0;

	(*current_skill_tree_panel)->SetVisibility(ESlateVisibility::Hidden);
	button->OnClicked.AddDynamic(this, &USkillCompanionToggleButton::ToggleSkillTreeUI);
}

ECharacterName USkillCompanionToggleButton::GetTargetECharacterName()
{
	return target_character_name;
}

void USkillCompanionToggleButton::SetSkillPoint(int32 point)
{
	skill_point = point;
	if(skill_point_text)
		skill_point_text->SetText(FText::Format(LOCTEXT("SkillPointText", "기술 점수: {:02}"), point));
}

#undef LOCTEXT_NAMESPACE