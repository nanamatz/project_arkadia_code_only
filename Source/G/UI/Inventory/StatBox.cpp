// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Inventory/StatBox.h"
#include "Components/TextBlock.h"
#include "G/Item/Ability.h"

void UStatBox::Update(FAbility& ability)
{
	strength_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Strength])));
	insight_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Insight])));
	swift_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Swift])));
	relaxed_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Relaxed])));
	careful_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Careful])));
	luck_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Luck])));
	power_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Power])));
	armor_value->SetText(FText::FromString(FString::SanitizeFloat(ability.Value[EAbility::Armor])));
}
