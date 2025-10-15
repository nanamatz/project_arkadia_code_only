// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include <G/GGameInstance.h>

#include "Save/SaveSystemUI.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Manager/InventoryManager.h"
#include "G/Manager/SaveManager.h"
#include "System/SystemUI.h"

void UMainMenuWidget::OnStartButtonClicked()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->GetInventoryManager()->ClearInventory();
	
	GameInstance->bIsStart = true;
	GameInstance->LoadStage1();
}

void UMainMenuWidget::OnLoadButtonClicked()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->bIsStart = false;
	GameInstance->GetSaveManager()->GetSaveUI()->On(false);
}

void UMainMenuWidget::OnSettingButtonClicked()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->GetSystemUI()->On();
}

bool UMainMenuWidget::Initialize()
{
	bool res = Super::Initialize();
	
	if (startButton)
	{
		startButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	}

	if (exitButton)
	{
		exitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	}

	if (loadButton)
	{
		loadButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadButtonClicked);
	}

	if(settingButton)
	{
		settingButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingButtonClicked);
	}
	return res;
}

void UMainMenuWidget::OnExitButtonClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(
		UGameplayStatics::GetPlayerController(GetGameInstance()->GetWorld(), 0));

	UKismetSystemLibrary::QuitGame(PlayerController->GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
