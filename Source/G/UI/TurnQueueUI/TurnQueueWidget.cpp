// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnQueueWidget.h"

#include "G/UI/InGameUI/InGameUI.h"
#include "G/UI/TurnQueueUI/TurnQueuePortrait.h"

#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "G/CharacterInfoComponent.h"
#include "G/GCharacter.h"
#include "G/UI/TurnQueueUI/TurnQueuePortraitTooltip.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UTurnQueueWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTurnQueueWidget::Init()
{
	tooltip = CreateWidget<UTurnQueuePortraitTooltip>(this, bp_turnqueue_tooltip);
	tooltip->Init(buff_texture_map, debuff_texture_map);
	tooltip_size_box->AddChild(tooltip);
	tooltip->Off();
}

void UTurnQueueWidget::EmphasisPortrait()
{
	UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(turn_queue_box->GetChildAt(0));
	if (portrait)
	{
		portrait->EmphasisCurrTurn();
	}
}

void UTurnQueueWidget::DeEmphasisPortrait()
{
	UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(turn_queue_box->GetChildAt(0));
	if (portrait)
	{
		portrait->DeEmphasis();
	}
}

void UTurnQueueWidget::InsertPortrait(AGCharacter* character)
{
	UTurnQueuePortrait* new_portrait = CreateWidget<UTurnQueuePortrait>(this, bp_turnqueue_portrait);

	UCanvasPanelSlot* cs = Cast<UCanvasPanelSlot>(turn_queue_box->Slot);

	cs->LayoutData.Anchors.Minimum.X -= 0.025f;
	cs->LayoutData.Anchors.Maximum.X += 0.025f;
	cs->LayoutData.Offsets.Left = 0;
	cs->LayoutData.Offsets.Right = 0;

	cs->SetLayout(cs->LayoutData);

	if (new_portrait)
	{
		UHorizontalBoxSlot* hb_slot = turn_queue_box->AddChildToHorizontalBox(new_portrait);

		hb_slot->SetSize(ESlateSizeRule::Fill);
		hb_slot->SetHorizontalAlignment(HAlign_Fill);
		hb_slot->SetVerticalAlignment(VAlign_Fill);

		hb_slot->Padding.Bottom = portrait_bottom_padding;
		hb_slot->SetPadding(hb_slot->Padding);

		new_portrait->Init(character, tooltip);

		for (FBUFF buff : character->GetCharacterInfoManager()->GetBuffListRef())
		{
			new_portrait->AddBuff(buff_texture_map[buff.type], &buff.turnCount);
		}

		for (FDEBUFF debuff : character->GetCharacterInfoManager()->GetDeBuffListRef())
		{
			new_portrait->AddBuff(debuff_texture_map[debuff.type], &debuff.turnCount);
		}
	}

	new_portrait->ProcessTurn();
}

void UTurnQueueWidget::DeletePortrait(int index)
{
	turn_queue_box->RemoveChildAt(index);

	UCanvasPanelSlot* cs = Cast<UCanvasPanelSlot>(turn_queue_box->Slot);

	cs->LayoutData.Anchors.Minimum.X += 0.025f;
	cs->LayoutData.Anchors.Maximum.X -= 0.025f;
	cs->LayoutData.Offsets.Left = 0;
	cs->LayoutData.Offsets.Right = 0;

	/* if SetLayout function isn't called, then layout will not be updated in delete case why??? */
	cs->SetLayout(cs->LayoutData);
}

void UTurnQueueWidget::FirstToLastPortrait()
{
	/* To prevent dangling reference */
	UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(turn_queue_box->GetChildAt(0));
	DeEmphasisPortrait();
	portrait->ProcessTurn();

	turn_queue_box->RemoveChildAt(0);

	UHorizontalBoxSlot* hb_slot = turn_queue_box->AddChildToHorizontalBox(portrait);
	hb_slot->SetSize(ESlateSizeRule::Fill);
	hb_slot->Padding.Bottom = portrait_bottom_padding;
	hb_slot->SetPadding(hb_slot->Padding);

	
}

void UTurnQueueWidget::ClearPortrait()
{
	for (int i = turn_queue_box->GetChildrenCount() - 1; i >= 0; i--)
	{
		/* Perhaps the Unreal Garbage Collector will remove the portrait widget, if not, then delete the portrait */
		turn_queue_box->RemoveChildAt(i);
	}

	UCanvasPanelSlot* cs = Cast<UCanvasPanelSlot>(turn_queue_box->Slot);
	cs->LayoutData.Anchors.Maximum.X = 0.5f;
	cs->LayoutData.Anchors.Minimum.X = 0.5f;
	cs->LayoutData.Offsets.Left = 0;
	cs->LayoutData.Offsets.Right = 0;
}

void UTurnQueueWidget::AddBuff(AGCharacter* character, FBUFF* buff)
{
	for (UWidget* _portrait : turn_queue_box->GetAllChildren())
	{
		UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(_portrait);

		if (portrait->target_character == character)
		{
			portrait->AddBuff(buff_texture_map[buff->type], &buff->turnCount);

			return;
		}
	}
}

void UTurnQueueWidget::AddDebuff(AGCharacter* character, FDEBUFF* debuff)
{
	for (UWidget* _portrait : turn_queue_box->GetAllChildren())
	{
		UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(_portrait);

		if (portrait->target_character == character)
		{
			portrait->AddDebuff(debuff_texture_map[debuff->type], &debuff->turnCount);

			return;
		}
	}
}

void UTurnQueueWidget::AddBuff(int32 index, FBUFF* buff)
{
	UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(turn_queue_box->GetChildAt(index));

	portrait->AddBuff(buff_texture_map[buff->type], &buff->turnCount);
}

void UTurnQueueWidget::AddDebuff(int32 index, FDEBUFF* debuff)
{
	UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(turn_queue_box->GetChildAt(index));

	portrait->AddBuff(debuff_texture_map[debuff->type], &debuff->turnCount);
}

void UTurnQueueWidget::UpdateHP(AGCharacter* character)
{
	for (UWidget* _portrait : turn_queue_box->GetAllChildren())
	{
		UTurnQueuePortrait* portrait = Cast<UTurnQueuePortrait>(_portrait);

		if (portrait->target_character == character)
		{
			portrait->UpdateHP(character->GetCurrentHP(), character->GetMaxHP(), character->GetCurrentShield());

			if (tooltip->GetTargetCharcter() == character)
			{
				tooltip->Update();
			}
		}
	}
}

/*
void UTurnQueueWidget::DeleteBuff(int32 index, FBUFF* buff) {
	
}

void UTurnQueueWidget::DeleteDebuff(int32 index, FDEBUFF* debuff) {
	
}
*/

void UTurnQueueWidget::Off()
{
	announcement_playerturn->SetVisibility(ESlateVisibility::Collapsed);
	tooltip->Off();
	this->RemoveFromViewport();
}

void UTurnQueueWidget::AnnouncementPlayerTurn()
{
	GetWorld()->GetTimerManager().ClearTimer(timer);
	announcement_playerturn->SetVisibility(ESlateVisibility::HitTestInvisible);
	announcement_text->SetText(FText(LOCTEXT("AnnouncementPlayerTurn", "플레이어 턴")));
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTurnQueueWidget::OffAnnouncement,
	                                       announcement_on_time, false);
}

void UTurnQueueWidget::AnnouncementStartCombat()
{
	GetWorld()->GetTimerManager().ClearTimer(timer);
	announcement_playerturn->SetVisibility(ESlateVisibility::HitTestInvisible);
	announcement_text->SetText(FText(LOCTEXT("AnnouncementPlayerTurn", "전투 시작")));

	if (combat_bgm)
	{
		audio_comp = UGameplayStatics::SpawnSound2D(this, combat_bgm);

		if (audio_comp)
		{
			audio_comp->Play();
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTurnQueueWidget::OffAnnouncement,
	                                       announcement_on_time, false);
}

void UTurnQueueWidget::AnnouncementEndCombat()
{
	GetWorld()->GetTimerManager().ClearTimer(timer);
	announcement_playerturn->SetVisibility(ESlateVisibility::HitTestInvisible);
	announcement_text->SetText(FText(LOCTEXT("AnnouncementPlayerTurn", "전투 종료")));

	if (audio_comp && audio_comp->IsPlaying())
	{
		audio_comp->Stop();
	}
	
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTurnQueueWidget::Off,
	                                       announcement_on_time, false);
}

void UTurnQueueWidget::OffAnnouncement() const
{
	announcement_playerturn->SetVisibility(ESlateVisibility::Collapsed);
}

#undef LOCTEXT_NAMESPACE
