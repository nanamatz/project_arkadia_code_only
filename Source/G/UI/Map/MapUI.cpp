// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUI.h"

#include <G/Manager/TutorialManager.h>

#include "G/UI/GameMenuManager.h"
#include "G/GGameInstance.h"
#include "G/GPlayerController.h"
#include "G/GGameMode.h"
#include "G/G.h"

#include "PointOfInterestWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "PointOfInterestComponent.h"
#include "WorldmapWidget.h"


void UMapUI::NativeConstruct()
{
	Super::NativeConstruct();

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Dimensions"), dimensions);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Zoom"), zoom);
	render_target_texture = render_target->ConstructTexture2D(render_target, "minimap_texture", RF_NoFlags,
	                                                          CTF_DeferCompression);
	map_img->GetDynamicMaterial()->SetTextureParameterValue(FName("MapImage"), render_target_texture);
}

FReply UMapUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	const auto key = InKeyEvent.GetKey();

	if (key == EKeys::W)
	{
		map_location.X += 100;
	}
	else if (key == EKeys::S)
	{
		map_location.X -= 100;
	}
	else if (key == EKeys::A)
	{
		map_location.Y -= 100;
	}
	else if (key == EKeys::D)
	{
		map_location.Y += 100;
	}
	else if (key == EKeys::M || key == EKeys::Escape)
	{
		UGameMenuManager* gm = Cast<UGameMenuManager>(GetOuter());

		if(gm)
		{
			LOG(Log, "gm valid");
			gm->Off();
		} else
		{
			UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->GetGameMenuManager()->Off();			
		}
		
		return FReply::Handled();
	}
	else if (key == EKeys::I || key == EKeys::K || key == EKeys::Q)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		GameInstance->GetGameMenuManager()->ChangeSubUI(key);

		return FReply::Handled();
	}

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Y"), map_location.X);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("X"), map_location.Y);

	worldmap_widget->Update(map_location, zoom);

	return FReply::Unhandled();
}

FReply UMapUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetEffectingButton();

	return reply;
}

FReply UMapUI::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetWheelDelta();

	if (key > 0 && zoom > 0.05f)
	{
		zoom -= key * 0.01f;
	}

	if (key < 0)
	{
		zoom -= key * 0.01f;
	}

	LOG(Log, "zoom:%f", zoom);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Zoom"), zoom);

	worldmap_widget->Update(map_location, zoom);

	return reply;
}

void UMapUI::On()
{
	Super::On();

	Cast<UGGameInstance>(GetGameInstance())->GetTutorialManager()->PlayTutorial(ETutorial::Map);
	
	SetFocus();

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Y"), map_location.X);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("X"), map_location.Y);
	
	worldmap_widget->Update(map_location, zoom);
}

void UMapUI::Off()
{
	Super::Off();

	worldmap_widget->Off();
}

void UMapUI::Init()
{
	map_location.X = 0;
	map_location.Y = 0;

	zoom = 1;

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Y"), map_location.X);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("X"), map_location.Y);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), map_data, FName("Zoom"), zoom);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	control_character = GameInstance->GetInGameDataManager()->GetControlCharacter();

	worldmap_widget->Init();
}

void UMapUI::CreateIcon()
{
}

float UMapUI::GetDimension()
{
	return dimensions;
}

float UMapUI::GetZoom()
{
	return zoom;
}

void UMapUI::SetZoom(float _zoom)
{
	zoom = _zoom;
}

UWorldmapWidget* UMapUI::GetWorldmapWidget()
{
	return worldmap_widget;
}
