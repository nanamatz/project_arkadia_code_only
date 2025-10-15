// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/TurnQueueUI/TurnQueuePortraitTooltip.h"

#include "TurnQueuePortraitTooltipStatus.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "G/CharacterInfoComponent.h"
#include "G/GCharacter.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UTurnQueuePortraitTooltip::UpdateBuffDeBuff()
{
	int32 idx = 0;

	for (FBUFF buff : cur_target_character->GetCharacterInfoManager()->GetBuffListRef())
	{
		UWidget* existing_status_box = status_box->GetChildAt(idx++);
		if (existing_status_box)
		{
			Cast<UTurnQueuePortraitTooltipStatus>(existing_status_box)->SetStatus(
				*buff_texture_map[buff.type], BuffTypeToFText(buff.type));
		}
		else
		{
			if (status_pool.Num() != 0)
			{
				UTurnQueuePortraitTooltipStatus* pooling_status_box = status_pool.Last();
				pooling_status_box->SetStatus(*buff_texture_map[buff.type], BuffTypeToFText(buff.type));
				status_box->AddChild(pooling_status_box);
				status_pool.RemoveAt(status_pool.Num() - 1);
			}
			else
			{
				UTurnQueuePortraitTooltipStatus* new_status = CreateWidget<UTurnQueuePortraitTooltipStatus>(
					this, bp_status);
				new_status->SetStatus(*buff_texture_map[buff.type], BuffTypeToFText(buff.type));
				status_box->AddChild(new_status);
			}
		}
	}

	for (FDEBUFF debuff : cur_target_character->GetCharacterInfoManager()->GetDeBuffListRef())
	{
		UWidget* existing_status_box = status_box->GetChildAt(idx++);

		if (existing_status_box)
		{
			Cast<UTurnQueuePortraitTooltipStatus>(existing_status_box)->SetStatus(
				*debuff_texture_map[debuff.type], DeBuffTypeToFText(debuff.type));
		}
		else
		{
			if (status_pool.Num() != 0)
			{
				UTurnQueuePortraitTooltipStatus* pooling_status_box = status_pool.Last();
				pooling_status_box->SetStatus(*debuff_texture_map[debuff.type], DeBuffTypeToFText(debuff.type));
				status_box->AddChild(pooling_status_box);
				status_pool.RemoveAt(status_pool.Num() - 1);
			}
			else
			{
				UTurnQueuePortraitTooltipStatus* new_status = CreateWidget<UTurnQueuePortraitTooltipStatus>(
					this, bp_status);
				new_status->SetStatus(*debuff_texture_map[debuff.type], DeBuffTypeToFText(debuff.type));
				status_box->AddChild(new_status);
			}
		}
	}

	int32 status_box_child_cnt = status_box->GetChildrenCount();

	if (idx < status_box_child_cnt)
	{
		for (int remove_idx = idx; remove_idx < status_box_child_cnt; remove_idx++)
		{
			UTurnQueuePortraitTooltipStatus* remove_widget = Cast<UTurnQueuePortraitTooltipStatus>(
				status_box->GetChildAt(0));
			status_pool.Add(remove_widget);
			status_box->RemoveChildAt(0);
		}
	}
}

void UTurnQueuePortraitTooltip::Init(TMap<EBUFFTYPE, UTexture2D*>& _buff_texture_map,
                                     TMap<EDEBUFFTYPE, UTexture2D*>& _debuff_texture_map)
{
	buff_texture_map = _buff_texture_map;
	debuff_texture_map = _debuff_texture_map;
}

void UTurnQueuePortraitTooltip::Update()
{
	if (cur_target_character)
	{
		FText name = cur_target_character->GetCharacterName();
		float current_hp = cur_target_character->GetCurrentHP();
		float max_hp = cur_target_character->GetMaxHP();

		name_text->SetText(FText::Format(LOCTEXT("TurnQueueTooltipNameText", "{0}"), name));
		hp_bar->SetPercent(current_hp / max_hp);
		hp->SetText(FText::Format(LOCTEXT("TurnQueueTooltipHP", "{0} / {1}"), current_hp, max_hp));

		UpdateBuffDeBuff();
	}
}

void UTurnQueuePortraitTooltip::On(AGCharacter* target_character)
{
	if (cur_target_character != target_character)
	{
		cur_target_character = target_character;

		FText name = target_character->GetCharacterName();
		float current_hp = target_character->GetCurrentHP();
		float max_hp = target_character->GetMaxHP();

		name_text->SetText(FText::Format(LOCTEXT("TurnQueueTooltipNameText", "{0}"), name));
		hp_bar->SetPercent(current_hp / max_hp);
		hp->SetText(FText::Format(LOCTEXT("TurnQueueTooltipHP", "{0} / {1}"), current_hp, max_hp));

		UpdateBuffDeBuff();

		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		cur_target_character = nullptr;

		Off();
	}
}

void UTurnQueuePortraitTooltip::Off()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

FText UTurnQueuePortraitTooltip::BuffTypeToFText(EBUFFTYPE& buff_type)
{
	switch (buff_type)
	{
	case EBUFFTYPE::COSTINCREASE:
		return FText(LOCTEXT("TurnQueueTooltipCI", "Cost Increase"));
	case EBUFFTYPE::LUCKYINCREASE:
		return FText(LOCTEXT("TurnQueueTooltipLI", "Luck Increase"));
	case EBUFFTYPE::SHIELDINCREASE:
		return FText(LOCTEXT("TurnQueueTooltipSI", "Shield Increase"));
	case EBUFFTYPE::INSIGHTINCREASE:
		return FText(LOCTEXT("TurnQueueTooltipII", "Insight Increase"));
	case EBUFFTYPE::MOVECOSTINCREASE:
		return FText(LOCTEXT("TurnQueueTooltipMCI", "Move Cost Increase"));
	case EBUFFTYPE::STRENGTHINCREASE:
		return FText(LOCTEXT("TurnQueueTooltipSI", "Strength Increase"));
	case EBUFFTYPE::INVULNERABILITY:
		return FText(LOCTEXT("TurnQueueTooltipIV", "Invulnerability"));
	default:
		UE_LOG(LogTemp, Log, TEXT("TurnQueueTooltip: BuffTypeToFText, no valid buff_type"))
	}

	return FText(LOCTEXT("TurnQueueTooltipDefault", "None"));
}

FText UTurnQueuePortraitTooltip::DeBuffTypeToFText(EDEBUFFTYPE& debuff_type)
{
	switch (debuff_type)
	{
	case EDEBUFFTYPE::STUN:
		return FText(LOCTEXT("TurnQueueTooltipStun", "Stun"));
	case EDEBUFFTYPE::DAMAGEDECREASE:
		return FText(LOCTEXT("TurnQueueTooltipDD", "Damage Decrease"));
	case EDEBUFFTYPE::WEAK:
		return FText(LOCTEXT("TurnQueueTooltipSD", "Shield Decrease"));
	case EDEBUFFTYPE::HEALINGDECREASE:
		return FText(LOCTEXT("TurnQueueTooltipHD", "Healing Decrease"));
	case EDEBUFFTYPE::APPLYPIERCEDAMAGE:
		return FText(LOCTEXT("TurnQueueTooltipAPD", "Pierced Damage"));
	default:
		UE_LOG(LogTemp, Log, TEXT("TurnQueueTooltip: DeBuffTypeToFText, no valid debuff_type"))
	}

	return FText(LOCTEXT("TurnQueueTooltipDefault", "None"));
}

AGCharacter* UTurnQueuePortraitTooltip::GetTargetCharcter()
{
	return cur_target_character;
}

#undef LOCTEXT_NAMESPACE
