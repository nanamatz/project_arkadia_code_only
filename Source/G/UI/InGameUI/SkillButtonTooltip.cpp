// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/InGameUI/SkillButtonTooltip.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "G/GCharacter.h"
#include "G/PlayerCharacter.h"
#include "G/Item/WeaponItemBase.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSlot.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void USkillButtonTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	FVector2D tooltip_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	tooltip_pos.X -= GetDesiredSize().X / 2;
	tooltip_pos.Y -= GetDesiredSize().Y + 10;
	SetPositionInViewport(tooltip_pos, false);
}

void USkillButtonTooltip::On(USkillSlot* mySkillSlot)
{
	if(!mySkillSlot)
	{
		return;
	}
	
	bHasScriptImplementedTick = true;

	AddToViewport();
	
	USkillBase* mySkill = mySkillSlot->GetMySkill();
	
	if(!mySkill)
	{
		return;
	}
		
	AGCharacter* myCharacter = mySkill->GetMyCharacter();

	if(!myCharacter)
	{
		return;
	}
	
	USkillInfoManager* SkillInfoManager = mySkill->GetSkillInfoManager();
	
	FVector2D tooltip_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	skill_name->SetText(SkillInfoManager->GetSkillName());
	cost->SetText(FText::Format(LOCTEXT("SkillButtonTooltipCost", "cost: {0}"), SkillInfoManager->GetCost()));
	auto attackList = SkillInfoManager->GetAttackList();
	auto buffList = SkillInfoManager->GetBuffList();
	auto debuffList = SkillInfoManager->GetDebuffList();

	int32 weaponDamage = 0;
	if(attackList.Num() > 0)
	{
		if(myCharacter->IsA(APlayerCharacter::StaticClass()))
		{
			auto weapon = Cast<APlayerCharacter>(myCharacter)->GetBoundWeapon();
			if(weapon)
			{
				weaponDamage = weapon->GetPower();
			}
		}
		damage->SetText(FText::Format(LOCTEXT("SkillButtonTooltipDamage", "Damage: {0}~{1} + {2}"), attackList[0].MinDamage, attackList[0].MaxDamage, weaponDamage));	
	}else
	{
		damage->SetText(FText::FromString(""));
	}

	{
		LOG(Log, "buffList");
		//horizontal_box->AddChildToHorizontalBox(new_button);
	}
	
	description->SetText(SkillInfoManager->GetDescription());
	ForceLayoutPrepass();
	tooltip_pos.X -= GetDesiredSize().X / 2;
	tooltip_pos.Y -= GetDesiredSize().Y + 10;

	SetPositionInViewport(tooltip_pos, false);
}

void USkillButtonTooltip::Off()
{
	bHasScriptImplementedTick = false;

	RemoveFromViewport();
}

#undef LOCTEXT_NAMESPACE
