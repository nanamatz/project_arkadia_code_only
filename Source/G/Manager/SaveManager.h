// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"

class USaveSystemUI;

/**
 * 
 */
UCLASS()
class G_API USaveManager : public UManagerBase
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<USaveSystemUI> bp_save_ui;

	UPROPERTY()
	USaveSystemUI* save_ui;

	UPROPERTY()
	TArray<FString> saved_folders;
	
	FString loaded_dir = "";
	int32 loaded_user_idx = 0;

private:
	void ReadAllFoldersInSaveGames();
	
public:
	USaveManager();

	virtual void Init(UGGameInstance* gameInstance) override;
	
	bool Save(FString parent_dir, int32 user_idx);
	void Load(FString parent_dir, int32 user_idx);

	USaveSystemUI* GetSaveUI();
	TPair<FString, int32> GetLoadInfo();
};
