// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/InGameUI/CircularPlayerPortrait.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/InGameDataManager.h"
#include "Kismet/GameplayStatics.h"

void UCircularPlayerPortrait::Init()
{
	Off();
}

void UCircularPlayerPortrait::SetPortrait(APlayerCharacter* pc)
{
	portrait->SetBrushFromMaterial(pc->GetCircularPortrait());

	character_name = pc->GetECharacterName();
	
	UpdateHP();
	On();
}

void UCircularPlayerPortrait::On()
{
	portrait->SetVisibility(ESlateVisibility::Visible);
	hp->SetVisibility(ESlateVisibility::Visible);
	hp_text->SetVisibility(ESlateVisibility::Visible);
}

void UCircularPlayerPortrait::Off()
{
	portrait->SetVisibility(ESlateVisibility::Collapsed);
	hp->SetVisibility(ESlateVisibility::Collapsed);
	hp_text->SetVisibility(ESlateVisibility::Collapsed);
}

void UCircularPlayerPortrait::UpdatePortrait()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto character = GameInstance->GetInGameDataManager()->GetControlCharacter();
	SetPortrait(character);
}
