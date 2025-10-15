// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/InGameUI/SkillButton.h"

#include "SkillButtonTooltip.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "G/GGameInstance.h"
#include "G/Controller/PlayerCombatController.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillData/SkillStruct.h"
#include "InGameUI.h"
#include "Kismet/GameplayStatics.h"


/*
void USkillButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Error, TEXT("SkillButton Created, NativeInitialzied"));
	skill_button->OnHovered.AddDynamic(this, &USkillButton::OnTooltip);
	skill_button->OnUnhovered.AddDynamic(this, &USkillButton::OffTooltip);
}*/

class UGGameInstance;

void USkillButton::OnHovered()
{
	if (skill_slot_ptr)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		FSkillStruct skillInfo = skill_slot_ptr->GetSkillInfo();
		GameInstance->GetInGameUI()->HighlightCostReduction(skillInfo.Cost);
		tooltip_ptr->On(skill_slot_ptr);
	}
}

void USkillButton::OnUnHovered()
{
	if (tooltip_ptr)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->GetInGameUI()->UnHighlightCostReduction();
		tooltip_ptr->Off();
	}
}

void USkillButton::OnButtonClick_AttackSet()
{
	UGameplayStatics::PlaySound2D(this, Cast<USoundBase>(button_clicked));
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickSkill) ||
		GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickTarget))
	{
		GameInstance->GetPlayerCombatController()->SkillPreProcessing(skill_num, this);
	}
}

void USkillButton::Init(USkillButtonTooltip* _tooltip_ptr, int32 _skill_num)
{
	skill_img->SetBrushSize(FVector2D(100, 100));
	skill_img->SetOpacity(0);

	skill_button->WidgetStyle.Normal.SetResourceObject(normal_skillButton);
	skill_button->WidgetStyle.Normal.Margin = 0;

	skill_button->WidgetStyle.Hovered.SetResourceObject(pressed_skillButton);
	skill_button->WidgetStyle.Hovered.Margin = 0;

	skill_button->WidgetStyle.Pressed.SetResourceObject(hovered_skillButton);
	skill_button->WidgetStyle.Pressed.Margin = 0;

	tooltip_ptr = _tooltip_ptr;
	skill_num = _skill_num;

	skill_button->OnClicked.AddDynamic(this, &USkillButton::OnButtonClick_AttackSet);
	skill_button->OnHovered.AddDynamic(this, &USkillButton::OnHovered);
	skill_button->OnUnhovered.AddDynamic(this, &USkillButton::OnUnHovered);

	cooldown_img->SetVisibility(ESlateVisibility::Collapsed);
	cooldown_text->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillButton::Set(USkillSlot* skill_slot, bool& isPlayerTurn)
{
	skill_button->SetIsEnabled(isPlayerTurn);

	skill_slot_ptr = skill_slot;
	skill_slot_ptr->SetSkillButton(this);

	skill_img->SetBrushFromTexture(skill_slot->GetThumnail());
	skill_img->SetOpacity(1);
	
	SetCoolDown();
}

void USkillButton::Clear()
{
	skill_slot_ptr = nullptr;
	skill_img->SetOpacity(0);
}

void USkillButton::SetCoolDown()
{
	if (skill_slot_ptr)
	{
		int cur_cooldown = skill_slot_ptr->GetMySkill()->GetSkillInfoManager()->GetCurCoolDown();

		if (cur_cooldown == 0)
		{
			OffCoolDown();

			return;
		}

		cooldown_img->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		cooldown_text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		cooldown_text->SetText(FText::FromString(FString::FromInt(cur_cooldown)));
	}
}

void USkillButton::OffCoolDown()
{
	cooldown_img->SetVisibility(ESlateVisibility::Collapsed);
	cooldown_text->SetVisibility(ESlateVisibility::Collapsed);
}
