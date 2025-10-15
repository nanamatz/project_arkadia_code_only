// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Skill/SkillUI.h"

#include <G/Manager/TutorialManager.h>

#include "SkillTreeNodeTooltip.h"
#include "SkillTreePanel.h"
#include "G/UI/GameMenuManager.h"
#include "G/PlayerCharacter.h"
#include "G/GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameMode.h"
#include "G/UI/Skill/SkillCompanionToggleButton.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "G/G.h"

void USkillUI::NativeConstruct()
{
	for(UWidget* _companion_toggle_bt : companion_toggle_box->GetAllChildren())
	{
		USkillCompanionToggleButton* companion_toggle_bt = Cast<USkillCompanionToggleButton>(_companion_toggle_bt);

		if(IsValid(companion_toggle_bt))
		{
			UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			int32 skill_point = GameInstance->GetInGameDataManager()->GetPlayerCharacterInfo(companion_toggle_bt->GetTargetECharacterName()).SkillPoint;
			
			companion_toggle_bt->SetSkillPoint(skill_point);
		}
	}
}

FReply USkillUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const auto key = InKeyEvent.GetKey();

	if (key == EKeys::K || key == EKeys::Escape)
	{
		UGameMenuManager* gm = Cast<UGameMenuManager>(GetOuter());

		if(gm)
		{
			LOG(Log, "gm valid");
			gm->Off();
		} else
		{
			LOG(Log, "gm non valid");
			UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->GetGameMenuManager()->Off();			
		}

		return FReply::Handled();
	}
	if (key == EKeys::I || key == EKeys::M || key == EKeys::Q)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		GameInstance->GetGameMenuManager()->ChangeSubUI(key);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}
void USkillUI::On()
{
	Super::On();

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	GameInstance->GetTutorialManager()->PlayTutorial(ETutorial::Skill);
}

void USkillUI::Off()
{
	Super::Off();

	tooltip->Off();
}

void USkillUI::Update()
{
}


void USkillUI::Init()
{
	tooltip = CreateWidget<USkillTreeNodeTooltip>(this, bp_tooltip);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	for (APlayerCharacter* character : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
	{
		AddCompanionToggleButton(character);
	}
}

void USkillUI::ClearCTB() const
{
	int32 child_cnt = companion_toggle_box->GetChildrenCount();

	for (int i = 0; i < child_cnt; i++)
	{
		companion_toggle_box->RemoveChildAt(0);
	}
}

void USkillUI::AddCompanionToggleButton(APlayerCharacter* character)
{
	for (auto _exist_ctb : companion_toggle_box->GetAllChildren())
	{
		USkillCompanionToggleButton* exist_ctb = Cast<USkillCompanionToggleButton>(_exist_ctb);
		if (character->GetECharacterName() == exist_ctb->GetTargetECharacterName())
		{
			return;
		}
	}

	USkillCompanionToggleButton* ctb = CreateWidget<USkillCompanionToggleButton>(this, bp_skill_toggle_button);
	ctb->skill_tree_panel = CreateWidget<USkillTreePanel>(this, bp_skill_tree_panel);

	companion_toggle_box->AddChildToVerticalBox(ctb);

	ctb->Init(skill_tree_overlay, &current_skill_tree_panel, character, tooltip);
}

USkillCompanionToggleButton* USkillUI::GetCTB(ECharacterName name)
{
	for(UWidget* _companion_toggle_bt : companion_toggle_box->GetAllChildren())
	{
		USkillCompanionToggleButton* companion_toggle_bt = Cast<USkillCompanionToggleButton>(_companion_toggle_bt);

		if(IsValid(companion_toggle_bt))
		{
			if(name == companion_toggle_bt->GetTargetECharacterName())
			{
				return companion_toggle_bt;
			}
		}
	}

	return nullptr;
}
