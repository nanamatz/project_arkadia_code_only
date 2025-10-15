// Fill out your copyright notice in the Description page of Project Settings.


#include "SystemUI.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "G/UI/GameMenuManager.h"
#include "G/GPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "SystemPanelBase.h"
#include "GraphicPanel.h"
#include "AudioPanel.h"
#include "ControlPanel.h"
#include "GamePlayPanel.h"
#include "KeySettingPanel.h"
#include "G/G.h"

FReply USystemUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const auto key = InKeyEvent.GetKey();

	if (key == EKeys::Escape)
	{
		UGameMenuManager* gm = Cast<UGameMenuManager>(GetOuter());

		Off();

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

bool USystemUI::Initialize()
{
	bool res = Super::Initialize();

	if (bp_graphic_panel)
	{
		graphic_panel = CreateWidget<UGraphicPanel>(this, bp_graphic_panel);
		graphic_button->OnClicked.AddDynamic(this, &USystemUI::OnGraphicPanel);
	}

	if (bp_audio_panel)
	{
		audio_panel = CreateWidget<UAudioPanel>(this, bp_audio_panel);
		audio_button->OnClicked.AddDynamic(this, &USystemUI::OnAudioPanel);
	}

	if (bp_key_setting_panel)
	{
		key_setting_panel = CreateWidget<UKeySettingPanel>(this, bp_key_setting_panel);
		key_button->OnClicked.AddDynamic(this, &USystemUI::OnKeySettingPanel);
	}

	if (bp_game_play_panel)
	{
		game_play_panel = CreateWidget<UGamePlayPanel>(this, bp_game_play_panel);
		gameplay_button->OnClicked.AddDynamic(this, &USystemUI::OnGamePlayPanel);
	}

	current_panel = graphic_panel;
	system_panel_border->AddChild(current_panel);

	if(apply_button)
		apply_button->OnClicked.AddDynamic(current_panel, &USystemPanelBase::Apply);

	if(exit_bt)
		exit_bt->OnClicked.AddDynamic(this, &USystemUI::Off);

	return res;
}

void USystemUI::Update()
{
}

void USystemUI::ChangePanel(int32 idx)
{
	LOG(Log, "CP");

	system_panel_border->RemoveChildAt(0);
	current_panel->Off();
	apply_button->OnClicked.RemoveDynamic(current_panel, &USystemPanelBase::Apply);

	switch (idx)
	{
	case 0:
		current_panel = graphic_panel;
		break;
	case 1:
		current_panel = audio_panel;
		break;
	case 3:
		current_panel = key_setting_panel;
		break;
	case 4:
		current_panel = game_play_panel;
		break;
	default:
		current_panel = graphic_panel;
	}

	apply_button->OnClicked.AddDynamic(current_panel, &USystemPanelBase::Apply);
	system_panel_border->AddChild(current_panel);
	current_panel->On();
}

void USystemUI::OnGraphicPanel()
{
	ChangePanel(0);
}

void USystemUI::OnAudioPanel()
{
	ChangePanel(1);
}

void USystemUI::OnKeySettingPanel()
{
	ChangePanel(3);
}

void USystemUI::OnGamePlayPanel()
{
	ChangePanel(4);
}

void USystemUI::On()
{
	Super::On();

	SetFocus();
	
	current_panel->On();
}

void USystemUI::Off()
{
	Super::Off();

	current_panel->Off();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
}