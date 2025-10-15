// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionCharacterInfoUI.h"
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include <Components/Border.h>
#include <Components/CanvasPanelSlot.h>
#include "GameFramework/GameUserSettings.h"

void UInteractionCharacterInfoUI::Init(FString Name, FVector2D TargetPos, FVector2D Offset)
{
	NameTextBlock->SetText(FText::FromString(Name));
	CanvasPanel->SetVisibility(ESlateVisibility::Hidden);

	UGameUserSettings* setting = GEngine->GetGameUserSettings();
	FIntPoint res = setting->GetScreenResolution();

	float XRatio = res.X / 1280.0f;
	float YRatio = res.Y / 720.0f;

	FVector2D middleOffset = FVector2D(Offset.X * XRatio, Offset.Y * YRatio);

	FVector2D Pos = TargetPos + middleOffset;

	this->SetPositionInViewport(Pos);

	if (Name.Len() > 9)
	{
		int32 len = Name.Len() - 10;
		float widthsize = 150.0f + ((len / 3) * 50.0f);

		Cast<UCanvasPanelSlot>(TextBorder->Slot)->SetSize(FVector2D(200.0f, 50.0f));
	}
	else
	{
		Cast<UCanvasPanelSlot>(TextBorder->Slot)->SetSize(FVector2D(150.0f, 50.0f));
	}

	//Cast<UCanvasPanelSlot>(TextBorder->Slot)->SetPosition(FVector2D(-150.0f, -25.0f));
}

void UInteractionCharacterInfoUI::Active()
{
	//this->SetPositionInViewport(Position);
	CanvasPanel->SetVisibility(ESlateVisibility::Visible);
	if (!isOn)
	{
		this->AddToViewport();
		isOn = true;
	}
}

void UInteractionCharacterInfoUI::DeActive()
{
	CanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	if (isOn)
	{
		this->RemoveFromViewport();
		isOn = false;
	}
}
