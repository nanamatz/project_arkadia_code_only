// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Manager/SaveManager.h"

#include "InGameDataManager.h"
#include "InventoryManager.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/QuestUI.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Dialog/FQuestManager.h"
#include "G/UI/Save/SaveSystemUI.h"

void USaveManager::ReadAllFoldersInSaveGames()
{
	FString save_games_dir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("SaveGames/*"));
	
	IFileManager& file_manager = IFileManager::Get();
	file_manager.FindFiles(saved_folders, *save_games_dir, false, true); // false: 파일 제외, true: 폴더만 포함

	for (const FString& folder : saved_folders)
	{
		UE_LOG(LogTemp, Log, TEXT("Found folder: %s"), *folder);
	}
}

USaveManager::USaveManager()
{
	ConstructorHelpers::FClassFinder<USaveSystemUI> _bp_save_ui(TEXT("WidgetBlueprint'/Game/UI/Save/WBP_SaveSystemUI.WBP_SaveSystemUI_C'"));
	
	save_ui = nullptr;
	
	if(_bp_save_ui.Succeeded())
	{
		bp_save_ui = _bp_save_ui.Class;
	}

	ReadAllFoldersInSaveGames();
}

void USaveManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
	
	save_ui = CreateWidget<USaveSystemUI>(gameInstance, bp_save_ui);

	save_ui->AddSavePath(saved_folders);
}

bool USaveManager::Save(FString parent_dir, int32 user_idx)
{
	FString default_path = FPaths::ProjectSavedDir() / TEXT("SaveGames/");
	FString saved_path = FPaths::Combine(default_path, parent_dir);

	if(IFileManager::Get().DirectoryExists(*saved_path))
	{
		return false;
	}
	
	if(IsValid(GameInstance))
	{
		GameInstance->GetInGameDataManager()->SaveData(user_idx, parent_dir);
		GameInstance->GetInventoryManager()->Save(parent_dir, user_idx);

		return true;
	}

	return false;
}

void USaveManager::Load(FString parent_dir, int32 user_idx)
{
	loaded_dir = parent_dir;
	loaded_user_idx = user_idx;

	GameInstance->QuestManager->InitQuestManager();
	GameInstance->GetInventoryManager()->ClearInventory();
	
	GameInstance->GetInGameDataManager()->Init(GameInstance);
	GameInstance->GetInGameDataManager()->LoadData(user_idx, parent_dir);
	GameInstance->GetInventoryManager()->Load(parent_dir, user_idx);
	GameInstance->GetGameMenuManager()->GetQuestUI()->ClearUI();
	GameInstance->GetGameMenuManager()->GetQuestUI()->Reconstitution();
	GameInstance->SetInputState(EInputState::State::Default);

	EGameLevel currentLevel = GameInstance->GetInGameDataManager()->GetCurrentLevel();

	switch (currentLevel)
	{
	case EGameLevel::Stage1:
		GameInstance->LoadStage1();
		break;
	case EGameLevel::CaveJail:
		GameInstance->LoadCaveJailLevel();
		break;
	case EGameLevel::Palace:
		GameInstance->LoadPalaceLevel();
		break;
	default:
		LOG(Error, "Not Find Level : %s", *UEnum::GetValueAsString(currentLevel));
	}
}

USaveSystemUI* USaveManager::GetSaveUI()
{
	return save_ui;
}

TPair<FString, int32> USaveManager::GetLoadInfo()
{
	return TPair<FString, int32>{loaded_dir, loaded_user_idx};
}