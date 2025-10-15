// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCompanionToggleButton.h"
#include "CharacterCaptureWidget.h"
#include "InventoryUI.h"
#include "G/PlayerCharacter.h"
#include "G/UI/GameMenuManager.h"
#include <Kismet/GameplayStatics.h>

#include "ExpBox.h"
#include "G/Manager/InGameDataManager.h"
#include "G/UI/Inventory/StatBox.h"

void UInventoryCompanionToggleButton::OnClicked()
{
	SetInventoryTarget();
}

void UInventoryCompanionToggleButton::Init(APlayerCharacter* player_character, ECharacterName* _target_character,
                                           UCharacterCaptureWidget* _ccw, UCharacterCaptureWidget** _cur_ccw)
{
	button->WidgetStyle.Normal.SetResourceObject(&player_character->GetPortrait());
	button->WidgetStyle.Normal.Margin = FMargin(0);
	button->WidgetStyle.Pressed.SetResourceObject(&player_character->GetPortrait());
	button->WidgetStyle.Pressed.Margin = FMargin(0);
	button->WidgetStyle.Hovered.SetResourceObject(&player_character->GetPortrait());
	button->WidgetStyle.Hovered.Margin = FMargin(0);

	button->OnClicked.AddDynamic(this, &UInventoryCompanionToggleButton::OnClicked);

	target_character = _target_character;
	bt_target_character = player_character->GetECharacterName();

	ccw = _ccw;
	cur_ccw = _cur_ccw;

	if (*cur_ccw)
	{
		_ccw->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		(*cur_ccw) = ccw;
	}
}

void UInventoryCompanionToggleButton::SetInventoryTarget()
{
	*target_character = bt_target_character;

	(*cur_ccw)->SetVisibility(ESlateVisibility::Hidden);
	ccw->SetVisibility(ESlateVisibility::Visible);

	(*cur_ccw) = ccw;

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	auto characterInfoManager = GameInstance->GetInGameDataManager()->GetPlayerCharacter(bt_target_character)->
											  GetCharacterInfoManager();

	UInventoryUI* InventoryUI = GameInstance->GetGameMenuManager()->GetInventoryUI();

	InventoryUI->stat_box->Update(characterInfoManager->GetAbilityRef());
	int32 characterLevel = characterInfoManager->GetLevel();
	InventoryUI->exp_box->Update(characterLevel, characterInfoManager->GetExpPercent());
}
