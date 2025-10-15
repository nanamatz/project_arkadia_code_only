// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Manager/DataManager.h"
#include "GGameInstance.generated.h"

class UGameMenuManager;
class UInGameUI;
class UDataManager;
class UInGameDataManager;
class UCombatManager;
class UPoolManager;
class UResourceManager;
class UPlayerCombatController;
class UPlayerMoveController;
class UInventoryManager;
class USystemUIManager;
class USystemUI;
class USaveSystemUI;
class USaveManager;
class UAnnotationWidget;
class UConfirmationBox;
class UImageAnnotationWidget;
class UFXManager;
class UTutorialManager;

class UPotionBase;

class UUserWidget;

/**
 * 
 * 플레이어가 선택한 캐릭터 정보를 저장해두고 게임 레벨에 들어갔을 때, 정보를 바탕으로 캐릭터 생성
 * 구조체 배열로 캐릭터 정보 저장
 * 
 */

UCLASS()
class G_API UGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<class UQuestInfo*> GQuestArray;
	UPROPERTY()
	TArray<UQuestInfo*> GDoingQuestArray;
	UPROPERTY()
	TArray<UQuestInfo*> GEndQuestArray;
	UPROPERTY()
	TArray<FString> GQuestHashTagArray;
private:
	/* GameMenuUI BP and Instance*/
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UGameMenuManager> _GameMenuManager;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UInGameUI> _InGameUI;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> bp_loading_screen;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USystemUI> bp_system;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UAnnotationWidget> bp_annotation_widget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UConfirmationBox> bp_confirm_box;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UImageAnnotationWidget> bp_img_annotation_widget;
	
	UPROPERTY()
	UAnnotationWidget* annotation_widget;

	UPROPERTY()
	UImageAnnotationWidget* img_annotation_widget;
	
	UPROPERTY()
	UConfirmationBox* confirm_box;
	
	UPROPERTY()
	UUserWidget* loading_screen;
	
	TSharedPtr<struct FStreamableHandle> streamable_handler;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUsableItemBase>> TestPotion;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotionBase> TestPotion1;

	UPROPERTY(VisibleInstanceOnly, Category = "Widget")
	UInGameUI* InGameUI;

	UPROPERTY(VisibleInstanceOnly, Category = "Widget")
	UGameMenuManager* GameMenuManager;

	UPROPERTY(Instanced)
	USaveManager* save_manager;

	FString main_menu_level = "World'/Game/Maps/LevelMainMenu.LevelMainMenu'";

	FName streaming_level_path;

	UPROPERTY()
	UWorld* streaming_world;

	int32 game_speed = 1.f;
	
protected:
	virtual void Init() override;

	UPROPERTY(Instanced)
	UDataManager* _dataManager;

	UPROPERTY(Instanced)
	UInGameDataManager* _inGameDataManager;

	UPROPERTY(Instanced)
	UCombatManager* _combatManager;

	UPROPERTY(Instanced)
	UPoolManager* _poolManager;

	UPROPERTY(Instanced)
	UResourceManager* _resourceManager;

	UPROPERTY(Instanced)
	UFXManager* _fxManager;

	UPROPERTY(Instanced)
	UTutorialManager* _tutorialManager;
	
	UPROPERTY(Instanced)
	UPlayerCombatController* _playerCombatController;

	UPROPERTY(Instanced)
	UPlayerMoveController* _playerMoveController;

	UPROPERTY(VisibleInstanceOnly)
	USystemUI* system_ui;

	EInputState::State _inputState;

public:
	UGGameInstance();

	UPROPERTY(BlueprintReadOnly)
	bool bIsStart = false;

private:
	void OpenLoadingScreen();
	void CloseLoadingScreen();

	UFUNCTION(BlueprintCallable)
	void SetCharacterLocationBeforeOpenLevel(FVector Location);
	
	void OpenStage1Start();
	void OpenMenuLevel();
	void OpenCaveJailLevel();
	void OpenPalaceLevel();
	
	UFUNCTION()
	void CompleteStreamingLevel();
	
public:
	UPROPERTY()
	UInventoryManager* inventory;

	UFUNCTION(BlueprintCallable)
	UInGameDataManager* GetInGameDataManager();
	void Update_UI(class APlayerCharacter* player_character);
	void ClearUI();
	
	UDataManager* GetDataManager();
	UFUNCTION(BlueprintCallable)
	UCombatManager* GetCombatManager();
	UInventoryManager* GetInventoryManager();
	UPoolManager* GetPoolManager();
	UResourceManager* GetResourceManager();
	UPlayerCombatController* GetPlayerCombatController();
	USaveManager* GetSaveManager();
	UFXManager* GetFXManager();
	UFUNCTION(BlueprintCallable)
	UTutorialManager* GetTutorialManager();

	UFUNCTION(BlueprintCallable)
	UPlayerMoveController* GetPlayerMoveController();

	UFUNCTION(BlueprintCallable)
	UInGameUI* GetInGameUI();

	UFUNCTION(BlueprintCallable)
	UGameMenuManager* GetGameMenuManager();

	UFUNCTION(BlueprintCallable)
	void LevelInit();

	APlayerCharacter* PlayerCharacterSpawnAndGet(ECharacterName name, FVector& spawnLocation,
	                                             FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f),
	                                             bool bNewPCharacter = false);

	IOnlineSessionPtr onlineSessionInterface;

	/* function related to InGameUI */
	void CreateInGameUI();


	UFUNCTION(BlueprintCallable)
	void OnInGameUI();

	UFUNCTION(BlueprintCallable)
	void OffInGameUI();

	/* function related to GameMenuUI */
	void CreateGameMenuUI();
	void OnGameMenuUI(enum class UI_TYPE ui);
	void OffGameMenuUI();
	enum class UI_TYPE get_current_ui();

	USystemUI* GetSystemUI() { return system_ui; }

	UFUNCTION(BlueprintCallable)
	UAnnotationWidget* GetAnnotationWidget() { return annotation_widget; }
	UConfirmationBox* GetConfirmBox() { return confirm_box; }
	UFUNCTION(BlueprintCallable)
	UImageAnnotationWidget* GetImageAnnotationWidget() { return img_annotation_widget; }

	UPROPERTY()
	class UQuestManager* QuestManager;

	void Quest_AddActiveQuest(int32 index);

	void Quest_AddArchivedQuest(int32 index);

	UFUNCTION(BlueprintCallable)
	void LoadStage1();
	UFUNCTION(BlueprintCallable)
	void LoadCaveJailLevel();
	UFUNCTION(BlueprintCallable)
	void LoadPalaceLevel();
	
	void LoadMenuLevel();
	void LoadStreamingLevel(UWorld* world, FName level_name);

	UFUNCTION(BlueprintCallable)
	bool GetIsStart() { return bIsStart; }
	UFUNCTION(BlueprintCallable)
	bool GetIsCombat();
	
	void SetInputState(EInputState::State newState) { _inputState = newState; }
	EInputState::State GetInputState() { return _inputState; }

	UFUNCTION(BlueprintCallable)
	void SetInputStateDefault() { _inputState = EInputState::State::Default; }

	UFUNCTION(BlueprintCallable)
	void AchieveReachPositionHashTag(FString tag);

	int32 IncreaseGameSpeed();
	int32 DecreaseGameSpeed();

	UFUNCTION(BlueprintCallable)
	void SetGameSpeed(int32 _game_speed);
};
