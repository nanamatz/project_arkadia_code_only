// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestIconUI.h"
#include <Engine/Texture2D.h>
#include <Components/Image.h>

void UQuestIconUI::UpdateQuestIcon(QuestIconState newstate)
{
	if (newstate == QuestIconState::NONE)
	{
		ImageIcon->SetVisibility(ESlateVisibility::Hidden);
		visible = false;
		return;
	}
	ImageIcon->SetVisibility(ESlateVisibility::Visible);
	visible = true;

	if (newstate == QuestIconState::ReadyIcon)
	{
		ImageIcon->SetBrushFromTexture(QuestReadyImage);
	}
	else if (newstate == QuestIconState::DoingIcon)
	{
		ImageIcon->SetBrushFromTexture(QuestDoingImage);
	}
	else if (newstate == QuestIconState::CompleteIcon)
	{
		ImageIcon->SetBrushFromTexture(QuestCompleteImage);
	}
}

void UQuestIconUI::UpdateVisible(bool rrr)
{
	if (!visible)
	{
		ImageIcon->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (rrr)
	{
		ImageIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ImageIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}
