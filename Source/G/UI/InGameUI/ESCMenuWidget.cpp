// Fill out your copyright notice in the Description page of Project Settings.


#include "ESCMenuWidget.h"
#include <Components/Button.h>

#include "G/GGameInstance.h"
#include "G/Manager/CombatManager.h"
#include "G/Manager/SaveManager.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/Save/SaveSystemUI.h"
#include "G/UI/System/SystemUI.h"
#include "Kismet/GameplayStatics.h"

class UGGameInstance;

bool UESCMenuWidget::Initialize()
{
	bool res = Super::Initialize();

	SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(resume_button))
	{
		resume_button->OnClicked.AddDynamic(this, &UESCMenuWidget::resume);
	}

	if (IsValid(save_button))
	{
		save_button->OnClicked.AddDynamic(this, &UESCMenuWidget::save);
	}

	if (IsValid(setting_button))
	{
		setting_button->OnClicked.AddDynamic(this, &UESCMenuWidget::setting);
	}

	if (IsValid(exit_button))
	{
		exit_button->OnClicked.AddDynamic(this, &UESCMenuWidget::exit);
	}

	return res;
}

void UESCMenuWidget::resume()
{
	Off();
}

void UESCMenuWidget::save()
{
	UGGameInstance* game_instance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	game_instance->GetSaveManager()->GetSaveUI()->On(true);
}

void UESCMenuWidget::setting()
{
	UGGameInstance* game_instance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SetVisibility(ESlateVisibility::Collapsed);

	game_instance->GetSystemUI()->On();
}

void UESCMenuWidget::exit()
{
	UGGameInstance* game_instance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	game_instance->GetCombatManager()->EndGame(true);
	Off();
	game_instance->LoadMenuLevel();
}

void UESCMenuWidget::On()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	
	SetVisibility(ESlateVisibility::Visible);
}

void UESCMenuWidget::Off()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(
		FInputModeGameAndUI().SetHideCursorDuringCapture(false));
	SetVisibility(ESlateVisibility::Collapsed);
}