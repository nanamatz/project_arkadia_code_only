// Fill out your copyright notice in the Description page of Project Settings.


#include "GGameInstance.h"

#include <G/Util/Define.h>

#include <G/Manager/DataManager.h>

#include "OnlineSubsystem.h"
#include "Controller/PlayerCombatController.h"
#include "Controller/PlayerMoveController.h"
#include "Kismet/GameplayStatics.h"

#include "G/Dialog/FQuestManager.h"

#include "G/UI/InGameUI/InGameUI.h"
#include "G/UI/Inventory/InventoryUI.h"
#include "G/Manager/InGameDataManager.h"
#include "G/UI/QuestUI.h"
#include "Manager/CombatManager.h"
#include "Manager/InventoryManager.h"
#include "UI/Skill/SkillUI.h"
#include "Blueprint/UserWidget.h"
#include <Engine/AssetManager.h>

#include "G.h"
#include "Engine/LevelStreaming.h"
#include "Item/PotionBase.h"
#include "Manager/FXManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SaveManager.h"
#include "Manager/TutorialManager.h"
#include "Manager/PoolManager/PoolManager.h"
#include "UI/GameMenuManager.h"
#include "UI/InGameUI/AnnotationWidget.h"
#include "UI/ImageAnnotationWidget.h"
#include "UI/ConfirmationBox.h"
#include "UI/Map/MapUI.h"
#include "UI/Map/WorldmapWidget.h"
#include "UI/System/SystemUI.h"

UGGameInstance::UGGameInstance()
{
}

void UGGameInstance::OpenLoadingScreen()
{
	if (!IsValid(loading_screen))
	{
		loading_screen = CreateWidget(this, bp_loading_screen);
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	loading_screen->AddToViewport();
}

void UGGameInstance::CloseLoadingScreen()
{
	if (IsValid(loading_screen))
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
		loading_screen->RemoveFromViewport();
	}
}

void UGGameInstance::SetCharacterLocationBeforeOpenLevel(FVector Location)
{
	for (auto& character : GetInGameDataManager()->GetPlayerCharacterList())
	{
		character->GetCharacterInfoManager()->SetLocation(Location);
		GetInGameDataManager()->SetSpawnPlayerCharacterInfoMap(character->GetCharacterInfo().EName, character->GetCharacterInfo());
	}
}

void UGGameInstance::OpenStage1Start()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("Stage1")));
	GetInGameDataManager()->ChangeCurrentLevel(EGameLevel::Stage1);
}

void UGGameInstance::OpenMenuLevel()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("LevelMainMenu")));
}

void UGGameInstance::OpenCaveJailLevel()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("CaveJail")));
	GetInGameDataManager()->ChangeCurrentLevel(EGameLevel::CaveJail);
}

void UGGameInstance::OpenPalaceLevel()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("Palace")));
	GetInGameDataManager()->ChangeCurrentLevel(EGameLevel::Palace);
}

void UGGameInstance::CompleteStreamingLevel()
{	
	CloseLoadingScreen();
}

void UGGameInstance::CreateInGameUI()
{
	if (_InGameUI)
	{
		InGameUI = Cast<UInGameUI>(CreateWidget(this, _InGameUI));

		if (InGameUI)
		{
			InGameUI->Init();
			for (auto a : GetInGameDataManager()->GetPlayerCharacterList())
			{
				InGameUI->AddPortrait(a);
			}
			InGameUI->AddToViewport();
		}
	}
}

void UGGameInstance::Update_UI(APlayerCharacter* player_character)
{
	GameMenuManager->GetInventoryUI()->AddCharacter(player_character);
	InGameUI->AddPortrait(player_character);
	GameMenuManager->GetSkillUI()->AddCompanionToggleButton(player_character);
	InGameUI->UpdateHP(player_character->GetECharacterName());
}

void UGGameInstance::ClearUI()
{
	InGameUI->ClearPortrait();
	GameMenuManager->GetInventoryUI()->ClearCaptureSlotAndCTB();
	GameMenuManager->GetSkillUI()->ClearCTB();
}

/* Make the InGameUI visible */
void UGGameInstance::OnInGameUI()
{
	InGameUI->AddToViewport();
}

/* Make the InGameUI hidden */
void UGGameInstance::OffInGameUI()
{
	InGameUI->RemoveFromViewport();
}

/* Create InGameUI */
void UGGameInstance::CreateGameMenuUI()
{
	if (_GameMenuManager)
	{
		GameMenuManager = Cast<UGameMenuManager>(CreateWidget(this, _GameMenuManager));
		if (GameMenuManager)
		{
			GameMenuManager->CreateSubUI(inventory->GetCurrentWeightRef(), inventory->GetMaxWeightRef());
			GameMenuManager->bIsFocusable = true;
		}
	}
}

void UGGameInstance::OnGameMenuUI(UI_TYPE ui)
{
	GameMenuManager->AddToViewport();
	GameMenuManager->ChangeSubUI(ui);
}

void UGGameInstance::OffGameMenuUI()
{
	GameMenuManager->RemoveFromViewport();
	GameMenuManager->OffCurrentUI();
}

UI_TYPE UGGameInstance::get_current_ui()
{
	return GameMenuManager->get_current_ui();
}

void UGGameInstance::Init()
{
	Super::Init();

	save_manager = NewObject<USaveManager>();
	save_manager->Init(this);		
	
	_dataManager = NewObject<UDataManager>();
	_dataManager->Init(this);

	_inGameDataManager = NewObject<UInGameDataManager>();
	_inGameDataManager->Init(this);

	_combatManager = NewObject<UCombatManager>();
	_combatManager->Init(this);

	_poolManager = NewObject<UPoolManager>();
	_poolManager->Init(this);

	_resourceManager = NewObject<UResourceManager>();
	_resourceManager->Init(this);
	
	_fxManager = NewObject<UFXManager>();
	_fxManager->Init(this);

	_tutorialManager = NewObject<UTutorialManager>();
	_tutorialManager->Init(this);
	
	// 퀘스트 매니저 초기화
	QuestManager = NewObject<UQuestManager>();
	QuestManager->Init(this);
	QuestManager->GenerateQuestInfo(this);
	_inputState = EInputState::State::Default;

	_playerCombatController = NewObject<UPlayerCombatController>();
	_playerCombatController->Init(this);

	_playerMoveController = NewObject<UPlayerMoveController>();
	_playerMoveController->Init(this);

	inventory = NewObject<UInventoryManager>(this);
	inventory->Init(this);

	if (bp_system)
	{
		system_ui = Cast<USystemUI>(CreateWidget(this, bp_system));
		system_ui->SetIsFocusable(true);
	}

	if(bp_annotation_widget)
	{
		annotation_widget = CreateWidget<UAnnotationWidget>(this, bp_annotation_widget);
	}

	if(bp_img_annotation_widget)
	{
		img_annotation_widget = CreateWidget<UImageAnnotationWidget>(this, bp_img_annotation_widget);
	}
	
	if(bp_confirm_box)
	{
		confirm_box = CreateWidget<UConfirmationBox>(this, bp_confirm_box);
	}
	
	CreateInGameUI();
	CreateGameMenuUI();

	inventory->SetInventoryUI(GameMenuManager->GetInventoryUI());
	inventory->SetQuestManager(QuestManager);

	for (auto testItem : TestPotion)
	{
		inventory->AddItem(NewObject<UUsableItemBase>(this, testItem));
	}
	inventory->AddItem(NewObject<UPotionBase>(this, TestPotion1));

	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	if (onlineSubsystem)
	{
		onlineSessionInterface = onlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Subsystem Name : %s"), *onlineSubsystem->GetSubsystemName().ToString()));
		}
	}
}

UDataManager* UGGameInstance::GetDataManager()
{
	return _dataManager;
}

UInGameDataManager* UGGameInstance::GetInGameDataManager()
{
	return _inGameDataManager;
}

void UGGameInstance::Quest_AddActiveQuest(int32 index)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance(263) - AddActiveQuest Execute, index : %d"), index);
	QuestManager->QuestDo(index);
	GameMenuManager->GetQuestUI()->Add_ActiveQuest(*(QuestManager->GetQuestList()[index]));
}

UCombatManager* UGGameInstance::GetCombatManager()
{
	return _combatManager;
}

UInventoryManager* UGGameInstance::GetInventoryManager()
{
	return inventory;
}

UPoolManager* UGGameInstance::GetPoolManager()
{
	return _poolManager;
}

UResourceManager* UGGameInstance::GetResourceManager()
{
	return _resourceManager;
}

UPlayerCombatController* UGGameInstance::GetPlayerCombatController()
{
	return _playerCombatController;
}

USaveManager* UGGameInstance::GetSaveManager()
{
	return save_manager;
}

UFXManager* UGGameInstance::GetFXManager()
{
	return _fxManager;
}

UTutorialManager* UGGameInstance::GetTutorialManager()
{
	return _tutorialManager;
}

UPlayerMoveController* UGGameInstance::GetPlayerMoveController()
{
	return _playerMoveController;
}

UInGameUI* UGGameInstance::GetInGameUI()
{
	return InGameUI;
}

UGameMenuManager* UGGameInstance::GetGameMenuManager()
{
	return GameMenuManager;
}

void UGGameInstance::LevelInit()
{
	GetCombatManager()->Init(this);

	ClearUI();
	OnInGameUI();
	
	LOG(Log, "bIsStart : %d", bIsStart);
	if (bIsStart)
	{
		GetInGameDataManager()->Init(this);
		_inGameDataManager->AddSpawnPlayerCharacterInfoMap(ECharacterName::Cardenio,
		                                                   *_dataManager->GetPlayerCharacterInfoData(
			                                                   ECharacterName::Cardenio));
		//_inGameDataManager->AddSpawnPlayerCharacterInfoMap(ECharacterName::Fernando,
		//												   *_dataManager->GetPlayerCharacterInfoData(
		//													   ECharacterName::Fernando));
	}
	// Save된 정보가 있으면 거기서 가져오기
	else
	{
		GetInGameDataManager()->ChangeLevel();
	}
}

APlayerCharacter* UGGameInstance::PlayerCharacterSpawnAndGet(ECharacterName name, FVector& spawnLocation,
                                                             FRotator spawnRotation, bool bNewCharacter)
{
	GetPlayerMoveController()->SetDestination(spawnLocation);

	LOG(Log, "PlayerCharacterSpawn : %s", *UEnum::GetValueAsString(name));
	// _spawnPlayerCharacterInfoMap에 있는지 확인
	
	TSubclassOf<APlayerCharacter> spawnCharacterClass = _dataManager->GetPlayerCharacterBP(name);
	// 무조건 생성은 해줘야 됨
	APlayerCharacter* spawnCharacter = GetWorld()->SpawnActor<APlayerCharacter>(spawnCharacterClass, spawnLocation, spawnRotation);
	spawnCharacter->InitCharacter();

	_inGameDataManager->AddPlayerCharacterMap(name, spawnCharacter, bNewCharacter);
	GetInGameDataManager()->SetControlCharacter(spawnCharacter);

	// UI initialized about PlayerCharacter
	InGameUI->UpdateControlCharacter();
	GameMenuManager->GetMapUI()->GetWorldmapWidget()->AddPlayerIcon(name);
	
	return spawnCharacter;
}


void UGGameInstance::Quest_AddArchivedQuest(int32 index)
{
	QuestManager->QuestComplete(index);
	GameMenuManager->GetQuestUI()->Add_ArchivedQuest(*(QuestManager->GetQuestList()[index]));
}

void UGGameInstance::AchieveReachPositionHashTag(FString tag)
{
	QuestManager->AchieveReachPositionHashTag(tag);
}

int32 UGGameInstance::IncreaseGameSpeed()
{
	if(game_speed < 3)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetWorldSettings()->SetTimeDilation(++game_speed);

	return game_speed;
}

int32 UGGameInstance::DecreaseGameSpeed()
{
	if(game_speed > 1)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetWorldSettings()->SetTimeDilation(--game_speed);

	return game_speed;
}

void UGGameInstance::SetGameSpeed(int32 _game_speed = 0)
{
	if(game_speed == 0)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetWorldSettings()->SetTimeDilation(game_speed);
	else
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetWorldSettings()->SetTimeDilation(_game_speed);
		
}

void UGGameInstance::LoadStage1()
{
	OpenLoadingScreen();

	FString path = MapPath + "Stage1/Stage1";
	FStringClassReference map(path);
	
	streamable_handler = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		map, FStreamableDelegate::CreateUObject(this, &UGGameInstance::OpenStage1Start));
}

void UGGameInstance::LoadCaveJailLevel()
{
	OpenLoadingScreen();

	FString path = MapPath + "CaveJail/CaveJail.CaveJail";
	FStringClassReference map(path);
	
	streamable_handler = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		map, FStreamableDelegate::CreateUObject(this, &UGGameInstance::OpenCaveJailLevel));
}

void UGGameInstance::LoadPalaceLevel()
{
	OpenLoadingScreen();
	
	FString path = MapPath + "Palace/Palace.Palace";
	FStringClassReference map(path);
	
	streamable_handler = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		map, FStreamableDelegate::CreateUObject(this, &UGGameInstance::OpenPalaceLevel));
}

void UGGameInstance::LoadMenuLevel()
{
	OpenLoadingScreen();

	FStringAssetReference map(main_menu_level);
	_inputState = EInputState::State::Default;
	GetInGameDataManager()->Init(this);
	GetGameMenuManager()->GetQuestUI()->ClearUI();
	QuestManager->InitQuestManager();
	streamable_handler = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		map, FStreamableDelegate::CreateUObject(this, &UGGameInstance::OpenMenuLevel));
}

void UGGameInstance::LoadStreamingLevel(UWorld* world, FName level_path)
{
	OpenLoadingScreen();

	if (!world)
	{
		LOG(Log, "world null");
		return;
	}

	if(GetWorld() == nullptr)
	{
		LOG(Log, "getworld null");
	}

	streaming_level_path = level_path;
	streaming_world = world;
	
	FLatentActionInfo LatentInfo;

	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName(TEXT("CompleteStreamingLevel"));
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;

	ULevelStreaming* streaming_level = UGameplayStatics::GetStreamingLevel(streaming_world, streaming_level_path);

	if (!streaming_level)
	{
		LOG(Log, "Level %s not found.", *streaming_level_path.ToString());
		return;
	}

	if (!streaming_level->HasLoadedLevel())
	{
		streaming_level->SetShouldBeLoaded(true);
		streaming_level->SetShouldBeVisible(true);
		streaming_level->bShouldBlockOnLoad = true;

		UGameplayStatics::LoadStreamLevel(world, level_path, true, true, LatentInfo);
		
		// world->FlushLevelStreaming(EFlushLevelStreamingType::Full);
	}
}

bool UGGameInstance::GetIsCombat()
{
	ECombatState combatState = GetCombatManager()->GetCombatState();
	return !(combatState == ECombatState::CombatNone || combatState == ECombatState::CombatEnd);
}
