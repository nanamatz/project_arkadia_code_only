// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Save/SaveSystemUI.h"

#include "SaveSlotWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Manager/InventoryManager.h"
#include "G/Manager/SaveManager.h"
#include "G/UI/ConfirmationBox.h"
#include "G/UI/InGameUI/AnnotationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/AllowWindowsPlatformTypes.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void USaveSystemUI::Save()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(!IsValid(GameInstance))
	{
		LOG(Log, "GameInstance Non-valid");
	}
	
	FString parent_dir = save_name_text->GetText().ToString();

	if(parent_dir != "")
	{
		if(!GameInstance->GetSaveManager()->Save(parent_dir, 0))
		{
			UConfirmationBox* confirm_box = GameInstance->GetConfirmBox();
			
			confirm_box->GetConfirmButton()->OnClicked.AddDynamic(this, &USaveSystemUI::SaveForce);
			confirm_box->GetCancelButton()->OnClicked.AddDynamic(this, &USaveSystemUI::SaveForceCancel);

			
			confirm_box->On(FText(LOCTEXT("SaveSystemUI", "동일한 이름의 파일이 존재합니다.\n덮어쓰시겠습니까?")));
			
			return;
		}	
	}

	AddSavePath(parent_dir);
}

void USaveSystemUI::Load()
{
	FString default_path = FPaths::ProjectSavedDir() / TEXT("SaveGames/");
	FString loading_path = FPaths::Combine(default_path, save_name_text->GetText().ToString());

	if(!IFileManager::Get().DirectoryExists(*loading_path))
	{
		return;
	}
	
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(!IsValid(GameInstance))
	{
		LOG(Log, "GameInstance Non-valid");

		return;
	}
	
	FString parent_dir = save_name_text->GetText().ToString();
	int32 user_idx = 0;
	
	GameInstance->GetSaveManager()->Load(parent_dir, user_idx);
}

bool USaveSystemUI::Initialize()
{
	bool res = Super::Initialize();
	
	save_bt->OnClicked.AddDynamic(this, &USaveSystemUI::Save);
	load_bt->OnClicked.AddDynamic(this, &USaveSystemUI::Load);
	exit_bt->OnClicked.AddDynamic(this, &USaveSystemUI::Off);
	
	return res;
}


void USaveSystemUI::On(bool save)
{
	save_bt->SetIsEnabled(save);
	
	AddToViewport();
}

void USaveSystemUI::Off()
{
	RemoveFromViewport();
}

void USaveSystemUI::AddSavePath(FString path)
{
	if(!bp_slot_widget)
	{
		LOG(Log, "BP_Slot_Widget in save system ui is non-valid");	
		return;
	}
	
	FString default_path = FPaths::ProjectSavedDir() / TEXT("SaveGames/");

	FString saved_path = FPaths::Combine(default_path, path);		
	FDateTime file_mod_date = IFileManager::Get().GetTimeStamp(*saved_path); 
		
	USaveSlotWidget* new_save_slot = CreateWidget<USaveSlotWidget>(this, bp_slot_widget);
		
	new_save_slot->Set(&save_name_text, path,file_mod_date);
		
	save_scroll_box->AddChild(new_save_slot);
}

void USaveSystemUI::AddSavePath(TArray<FString> paths)
{
	if(!bp_slot_widget)
	{
		LOG(Log, "BP_Slot_Widget in save system ui is non-valid");	
		return;
	}
	
	FString default_path = FPaths::ProjectSavedDir() / TEXT("SaveGames/");
	
	for(FString path : paths)
	{
		FString saved_path = FPaths::Combine(default_path, path);		
		FDateTime file_mod_date = IFileManager::Get().GetTimeStamp(*saved_path); 
		
		USaveSlotWidget* new_save_slot = CreateWidget<USaveSlotWidget>(this, bp_slot_widget);
		
		new_save_slot->Set(&save_name_text, path,file_mod_date);
		
		save_scroll_box->AddChild(new_save_slot);
	}
}

void USaveSystemUI::SaveForce()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	LOG(Log, "")
	
	if(IsValid(GameInstance))
	{
		GameInstance->GetInGameDataManager()->SaveData(0, save_name_text->GetText().ToString());
		GameInstance->GetInventoryManager()->Save(save_name_text->GetText().ToString(), 0);
	}
	
	UConfirmationBox* confirm_box = GameInstance->GetConfirmBox();
			
	confirm_box->GetConfirmButton()->OnClicked.RemoveDynamic(this, &USaveSystemUI::SaveForce);
	confirm_box->GetCancelButton()->OnClicked.RemoveDynamic(this, &USaveSystemUI::SaveForceCancel);	
}

void USaveSystemUI::SaveForceCancel()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	UConfirmationBox* confirm_box = GameInstance->GetConfirmBox();
			
	confirm_box->GetConfirmButton()->OnClicked.RemoveDynamic(this, &USaveSystemUI::SaveForce);
	confirm_box->GetCancelButton()->OnClicked.RemoveDynamic(this, &USaveSystemUI::SaveForceCancel);
}

#undef LOCTEXT_NAMESPACE
