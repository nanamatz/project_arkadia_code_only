// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/InteractionIcon.h"
#include "GameFramework/GameUserSettings.h"

void UInteractionIcon::SetVisible(bool vis)
{
	if (vis)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInteractionIcon::OnInteractIcon(FVector2D pos, FVector2D offset)
{
	TargetPos = pos;
	Offset = offset;
	CoordinateCalculation();

	if (!isOn)
	{
		this->AddToViewport();
		isOn = true;
	}
}

void UInteractionIcon::OffInteractIcon()
{
	if (isOn)
	{
		this->RemoveFromViewport();
		isOn = false;
	}
}

void UInteractionIcon::CoordinateCalculation()
{
	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	FIntPoint res = setting->GetScreenResolution();

	float XRatio = res.X / 1280.0f;
	float YRatio = res.Y / 720.0f;

	FVector2D middleOffset = FVector2D(Offset.X * XRatio, Offset.Y * YRatio);

	FVector2D result = FVector2D(0.0f, 0.0f);	
	result = TargetPos + middleOffset;

	this->SetPositionInViewport(result);
}