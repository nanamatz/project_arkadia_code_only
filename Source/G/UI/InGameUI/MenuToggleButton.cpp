// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuToggleButton.h"

#include "G/GGameInstance.h"
#include "G/UI/GameMenuManager.h"
#include "Kismet/GameplayStatics.h"

void UMenuToggleButton::OnGameMenu()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->GetGameMenuManager()->On(static_cast<UI_TYPE>(index));
}

UMenuToggleButton::UMenuToggleButton()
{
	OnClicked.AddDynamic(this, &UMenuToggleButton::OnGameMenu);
}
