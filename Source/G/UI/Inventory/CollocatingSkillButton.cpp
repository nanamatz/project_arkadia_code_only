// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Inventory/CollocatingSkillButton.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "G/Item/ItemBase.h"
#include "G/Item/UsableItemBase.h"

/*void UCollocatingSkillButton::Collocate()
{
	if (this->skill_slot_ptr != nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("이미 설정된 슬롯")));
		}

		return;
	}

	if (*CurrentCollocatedButton)
	{
		(*CurrentCollocatedButton)->Clear();
	}

	*CurrentCollocatedButton = this;

	if (*Item)
	{
		skill_img->SetBrushFromTexture((*Item)->GetIcon());
		skill_img->SetOpacity(1);
	}
}*/

void UCollocatingSkillButton::SetIcon(UTexture2D* icon)
{
	skill_img->SetBrushFromTexture(icon);
	skill_img->SetOpacity(1);
}

void UCollocatingSkillButton::Init(USkillButtonTooltip* _tooltip_ptr, int32 _skill_num)
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

	skill_button->OnHovered.AddDynamic(this, &UCollocatingSkillButton::OnHovered);
	skill_button->OnUnhovered.AddDynamic(this, &UCollocatingSkillButton::OnUnHovered);

	cooldown_img->SetVisibility(ESlateVisibility::Collapsed);
	cooldown_text->SetVisibility(ESlateVisibility::Collapsed);
}
