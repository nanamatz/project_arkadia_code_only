// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Inventory/ExpBox.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "G/G.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UExpBox::Update(int32& level, float exp_percent)
{
	level_text->SetText(FText::Format(LOCTEXT("ExpBoxLevel", "Lv.{0}"), level));
	exp_bar->SetPercent(exp_percent);
}

void UExpBox::SetExp(float exp_percent)
{
	exp_bar->SetPercent(exp_percent);
}

void UExpBox::SetLevel(int32& level)
{
	level_text->SetText(FText::Format(LOCTEXT("ExpBoxLevel", "Lv.{0}"), level));
}

#undef LOCTEXT_NAMESPACE
