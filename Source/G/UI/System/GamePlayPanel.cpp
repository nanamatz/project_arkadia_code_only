// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayPanel.h"

#include "HorizontalComboBox.h"
#include "G/G.h"
#include "G/GPlayerCamera.h"
#include "G/Save/SettingSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UGamePlayPanel::Save()
{
	FString slot_name = TEXT("SettingSaveSlot");
	int32 user_idx = 0;
	
	USettingSaveGame* save = Cast<USettingSaveGame>(UGameplayStatics::LoadGameFromSlot(slot_name, user_idx));

	if(!save)
	{
		save = Cast<USettingSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingSaveGame::StaticClass()));
	}
	
	save->Save(slot_name, user_idx, cam_mov_speed->GetSelectedOptionIdx(), cam_rot_speed->GetSelectedOptionIdx(), language->GetSelectedOption()); 
}

void UGamePlayPanel::Load()
{
	FString SlotName = TEXT("SettingSaveSlot");
	int32 UserIndex = 0;
	
	USettingSaveGame* save = Cast<USettingSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if(!IsValid(save))
		return;
	
	cam_mov_speed->SetSelectedOption(save->GetMoveLevel());
	cam_rot_speed->SetSelectedOption(save->GetRotLevel());
	language->SetSelectedOption(save->GetLanguage());

	Apply();
}

bool UGamePlayPanel::Initialize()
{
	bool res = Super::Initialize();
	
	if(cam_mov_speed_arr.Num() != 5)
	{
		LOG(Log, "SetNum");
		cam_mov_speed_arr = { 0.5f, 0.75f, 1, 1.25f, 1.5f };
	}

	if(cam_rot_speed_arr.Num() != 5)
	{
		LOG(Log, "SetNum");
		cam_rot_speed_arr = { 2.5f, 5, 10, 20, 30 };
	}
	
	Load();
	
	return res;
}

void UGamePlayPanel::On()
{
	Super::On();
}

void UGamePlayPanel::Off()
{
	Super::Off();
}

void UGamePlayPanel::Apply()
{
	Super::Apply();

	AGPlayerCamera* camera = Cast<AGPlayerCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (camera)
	{
		camera->SetCameraMovSpeed(cam_mov_speed_arr[cam_mov_speed->GetSelectedOptionIdx()]);
		camera->SetCameraRotSpeed(cam_rot_speed_arr[cam_rot_speed->GetSelectedOptionIdx()]);
	}

	LOG(Log, "%s %s", *language->GetSelectedOption().ToString(), *FString("한국어"));
	
	if(language->GetSelectedOptionIdx() == 0)
	{
		LOG(Log, "Set ko-KR")
		FInternationalization::Get().SetCurrentCulture("ko-KR");
	} else 
	{
		LOG(Log, "Set en")
		FInternationalization::Get().SetCurrentCulture("en");
	}
	
	cam_mov_speed->Apply();
	cam_rot_speed->Apply();
	language->Apply();
	
	Save();
}
