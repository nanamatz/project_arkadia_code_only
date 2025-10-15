// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestComponent.h"

#include "WorldmapWidget.h"
#include "G/GGameInstance.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/InGameUI/InGameUI.h"
#include "MapUI.h"
#include "MapWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPointOfInterestComponent::UPointOfInterestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPointOfInterestComponent::BeginPlay()
{
	Super::BeginPlay();
}

UTexture2D* UPointOfInterestComponent::GetIconImage()
{
	return icon_image;
}

void UPointOfInterestComponent::AddPOI()
{
	if (!is_register)
	{
		is_register = true;
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->GetInGameUI()->GetMinimap()->AddPOI(GetOwner());
		GameInstance->GetGameMenuManager()->GetMapUI()->GetWorldmapWidget()->AddPoi(GetOwner());
	}
}

void UPointOfInterestComponent::SetIcon(UTexture2D* texture)
{
	icon_image = texture;
}
