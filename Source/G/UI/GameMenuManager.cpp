// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/GameMenuManager.h"

#include "EnhenceUI.h"
#include "InGameUI/InGameUI.h"
#include "Map/MapUI.h"
#include "QuestUI.h"
#include "System/SystemUI.h"
#include "Components/Button.h"
#include "G/GGameInstance.h"
#include "G/GPlayerController.h"
#include "G/Manager/InventoryManager.h"
#include "Inventory/InventoryUI.h"
#include "Looting/LootingUI.h"
#include "Kismet/GameplayStatics.h"
#include "Looting/LootingComponent.h"
#include "Skill/SkillUI.h"
#include "InteractionIcon.h"

class UGGameInstance;

FReply UGameMenuManager::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const auto key = InKeyEvent.GetKey();

	if (key == EKeys::Escape || key == SubUIKey())
	{
		Off();

		return FReply::Handled();
	}
	if (key == EKeys::I || key == EKeys::K || key == EKeys::M || key == EKeys::Q)
	{
		ChangeSubUI(key);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FKey UGameMenuManager::SubUIKey()
{
	switch (current_ui)
	{
	case UI_TYPE::Inventory:
		return EKeys::I;

	case UI_TYPE::Map:
		return EKeys::M;

	case UI_TYPE::Quest:
		return EKeys::Q;

	case UI_TYPE::Skill:
		return EKeys::K;

	default:
		return FKey();
	}

	return FKey();
}

bool UGameMenuManager::Initialize()
{
	bool res = Super::Initialize();

	if(exit_bt)
	{
		exit_bt->OnClicked.AddDynamic(this, &UGameMenuManager::Off);
	}
	
	return res;
}

void UGameMenuManager::Init()
{
	UE_LOG(LogTemp, Log, TEXT("GameMenuManager Init"));
	current_ui = UI_TYPE::None;

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (bp_inventory)
	{
		inventory = Cast<UInventoryUI>(CreateWidget(GetWorld(), bp_inventory));
		inventory->Init(GameInstance->inventory->GetCurrentWeightRef(), GameInstance->inventory->GetMaxWeightRef());
		inventory->bIsFocusable = true;
	}

	if (bp_skill)
	{
		skill = Cast<USkillUI>(CreateWidget(GetWorld(), bp_skill));
		skill->Init();
		skill->bIsFocusable = true;
	}

	if (bp_quest)
	{
		quest = Cast<UQuestUI>(CreateWidget(GetWorld(), bp_quest));
		quest->Init();
		quest->bIsFocusable = true;
	}

	if (bp_map)
	{
		map = Cast<UMapUI>(CreateWidget(GetWorld(), bp_map));
		map->Init();
		map->bIsFocusable = true;
	}

	if (bp_enhence)
	{
		enhence = Cast<UEnhenceUI>(CreateWidget(GetWorld(), bp_enhence));
		enhence->init();
		enhence->bIsFocusable = true;
	}

	if (bp_looting)
	{
		looting = Cast<ULootingUI>(CreateWidget(GetWorld(), bp_looting));
		looting->init();
		looting->bIsFocusable = true;
	}

	if (bp_intericon)
	{
		intericon = Cast<UInteractionIcon>(CreateWidget(GetWorld(), bp_intericon));
		intericon->bIsFocusable = true;
	}

	inventory_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToInventory);
	skill_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToSkill);
	quest_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToQuest);
	map_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToMap);

	SetIsFocusable(false);
}

/// <summary>
/// When the character is recruited, then call this function.
/// Update the New Character Info about whole SubUI.
/// </summary>
void UGameMenuManager::Update(APlayerCharacter* player_character) const
{
	inventory->AddCharacter(player_character);
	skill->Update();
}

void UGameMenuManager::Clear() const
{
	/* Inventory UI is cleared by inventory manager */
	skill->ClearCTB();
}

void UGameMenuManager::On(UI_TYPE UI)
{
	ChangeSubUI(UI);
	AddToViewport();
}

void UGameMenuManager::Off()
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(GetGameInstance()->GetWorld(), 0));

	if (PlayerController)
	{
		UInGameUI* InGameUI = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetInGameUI();

		InGameUI->On();

		OffCurrentUI();
		RemoveFromViewport();


		PlayerController->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
	}
}

void UGameMenuManager::OffCurrentUI()
{
	switch (current_ui)
	{
	case UI_TYPE::Inventory:
		inventory->Off();
		break;

	case UI_TYPE::Skill:
		skill->Off();
		break;

	case UI_TYPE::Quest:
		quest->Off();
		break;

	case UI_TYPE::Map:
		map->Off();
		break;
	}
}

void UGameMenuManager::CreateSubUI(int32& current_weight, int32& max_weight)
{
	current_ui = UI_TYPE::None;

	if (bp_inventory)
	{
		inventory = Cast<UInventoryUI>(CreateWidget(GetWorld(), bp_inventory));
		inventory->Init(current_weight, max_weight);
		inventory->bIsFocusable = true;
	}

	if (bp_skill)
	{
		skill = Cast<USkillUI>(CreateWidget(GetWorld(), bp_skill));
		skill->Init();
		skill->bIsFocusable = true;
	}

	if (bp_quest)
	{
		quest = Cast<UQuestUI>(CreateWidget(GetWorld(), bp_quest));
		quest->Init();
		quest->bIsFocusable = true;
	}

	if (bp_map)
	{
		map = Cast<UMapUI>(CreateWidget(GetWorld(), bp_map));
		map->Init();
		map->bIsFocusable = true;
	}

	if (bp_enhence)
	{
		enhence = Cast<UEnhenceUI>(CreateWidget(GetWorld(), bp_enhence));
		enhence->init();
		enhence->bIsFocusable = true;
	}

	if (bp_looting)
	{
		looting = Cast<ULootingUI>(CreateWidget(GetWorld(), bp_looting));
		looting->init();
		looting->bIsFocusable = true;
	}

	if (bp_intericon)
	{
		intericon = Cast<UInteractionIcon>(CreateWidget(GetWorld(), bp_intericon));
		intericon->bIsFocusable = true;
	}

	inventory_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToInventory);
	skill_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToSkill);
	quest_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToQuest);
	map_button->OnClicked.AddDynamic(this, &UGameMenuManager::ChangeToMap);
}

void UGameMenuManager::ChangeSubUI(UI_TYPE ui)
{
	OffCurrentUI();
	current_ui = ui;

	switch (current_ui)
	{
	case UI_TYPE::Inventory:
		inventory->On();
		break;
	case UI_TYPE::Skill:
		skill->On();
		break;
	case UI_TYPE::Quest:
		quest->On();
		break;
	case UI_TYPE::Map:
		map->On();
		break;
	}
}

void UGameMenuManager::ChangeSubUI(const FKey& key)
{
	OffCurrentUI();

	if (key == EKeys::I)
	{
		current_ui = UI_TYPE::Inventory;
		inventory->On();
		inventory->SetKeyboardFocus();
	}
	else if (key == EKeys::K)
	{
		current_ui = UI_TYPE::Skill;
		skill->On();
		skill->SetKeyboardFocus();
	}
	else if (key == EKeys::Q)
	{
		current_ui = UI_TYPE::Quest;
		quest->On();
		quest->SetKeyboardFocus();
	}
	else if (key == EKeys::M)
	{
		current_ui = UI_TYPE::Map;
		map->On();
		map->SetKeyboardFocus();
	}
}

void UGameMenuManager::ChangeToInventory()
{
	ChangeSubUI(UI_TYPE::Inventory);
}

void UGameMenuManager::ChangeToSkill()
{
	ChangeSubUI(UI_TYPE::Skill);
}

void UGameMenuManager::ChangeToQuest()
{
	ChangeSubUI(UI_TYPE::Quest);
}

void UGameMenuManager::ChangeToMap()
{
	ChangeSubUI(UI_TYPE::Map);
}

UInventoryUI* UGameMenuManager::GetInventoryUI()
{
	return inventory;
}

USkillUI* UGameMenuManager::GetSkillUI()
{
	return skill;
}

UQuestUI* UGameMenuManager::GetQuestUI()
{
	return quest;
}

UMapUI* UGameMenuManager::GetMapUI()
{
	return map;
}

UEnhenceUI* UGameMenuManager::GetEnhenceUI()
{
	return enhence;
}

UInteractionIcon* UGameMenuManager::GetInteractionIcon()
{
	return intericon;
}

void UGameMenuManager::AddCharacterCaptureWidget(UCharacterCaptureWidget* ccw)
{
	inventory->AddCharacterCaptureWidget(ccw);
}

void UGameMenuManager::OnEnhenceUI()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SetInputState(EInputState::UI);
	enhence->Update();
	enhence->AddToViewport();
}

void UGameMenuManager::OffEnhenceUI()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SetInputState(EInputState::Default);
	enhence->RemoveFromViewport();
}

void UGameMenuManager::OnLootingUI(FVector2D pos)
{
	looting->OpenLootingUI(pos);
}

void UGameMenuManager::OffLootingUI()
{
	looting->RemoveFromViewport();
}

void UGameMenuManager::SetLootingUI(ULootingComponent* ltc)
{
	looting->SetItemList(ltc);
}
