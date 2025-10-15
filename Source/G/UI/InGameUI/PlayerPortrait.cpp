// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/InGameUI/PlayerPortrait.h"
#include "G/GGameInstance.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/InGameDataManager.h"

#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include <Kismet/GameplayStatics.h>

#include "G/G.h"


#define LOCTEXT_NAMESPACE "Arkadia"

void UPlayerPortrait::Init(APlayerCharacter* pc)
{
	character_name = pc->GetECharacterName();

	portrait->SetBrushFromTexture(&pc->GetPortrait());

	UpdateHP();
}

void UPlayerPortrait::SetHP(float curr_hp, float max_hp)
{
	hp->SetPercent(1 - curr_hp / max_hp);
	hp_text->SetText(FText::Format(LOCTEXT("PlayerPortraitHPText", "{0} / {1}"), curr_hp, max_hp));
}

void UPlayerPortrait::UpdateHP()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerCharacter* pc = GameInstance->GetInGameDataManager()->GetPlayerCharacter(character_name);

	if(!pc)
	{
		LOG(Log, "player character non valid error");
		return;
	}
		
	
	float curr_hp = pc->GetCurrentHP();
	float max_hp = pc->GetMaxHP();

	hp->SetPercent(1 - curr_hp / max_hp);
	hp_text->SetText(FText::Format(LOCTEXT("PlayerPortraitHPText", "{0} / {1}"), curr_hp, max_hp));
}

void UPlayerPortrait::UpdateHP(ECharacterName characterName)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerCharacter* pc = GameInstance->GetInGameDataManager()->GetPlayerCharacter(characterName);

	if(character_name != characterName)
	{
		return;
	}
	
	if(!pc)
	{
		LOG(Log, "player character non valid error");
		return;
	}
		
	
	float curr_hp = pc->GetCurrentHP();
	float max_hp = pc->GetMaxHP();

	hp->SetPercent(1 - curr_hp / max_hp);
	hp_text->SetText(FText::Format(LOCTEXT("PlayerPortraitHPText", "{0} / {1}"), curr_hp, max_hp));
}

#undef LOCTEXT_NAMESPACE
