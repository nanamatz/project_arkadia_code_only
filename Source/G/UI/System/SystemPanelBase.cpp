// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemPanelBase.h"

bool USystemPanelBase::Initialize()
{
	bool res = Super::Initialize();

	return res;
}

void USystemPanelBase::On()
{
	SetVisibility(ESlateVisibility::Visible);
}

void USystemPanelBase::Off()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void USystemPanelBase::Apply()
{
}
