// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestWidget.h"

#include "PointOfInterestComponent.h"

#include "G/PlayerCharacter.h"
#include "G/UI/GameMenuManager.h"

#include "Components/Image.h"
#include "Components/Throbber.h"
#include "G/G.h"

class UGGameInstance;

void UPointOfInterestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!IsValid(owner))
	{
		LOG(Log, "owner non valid");

		return;
	}
	UPointOfInterestComponent* poi_comp = Cast<UPointOfInterestComponent>(
		owner->GetComponentByClass(UPointOfInterestComponent::StaticClass()));

	if (poi_comp)
	{
		UE_LOG(LogTemp, Log, TEXT("POI Valid"));
		UTexture2D* IconImage = poi_comp->GetIconImage();
		if (IconImage)
		{
			UE_LOG(LogTemp, Log, TEXT("IconImage Valid"));
			if (custom_image)
			{
				UE_LOG(LogTemp, Log, TEXT("CustomImage Valid"));
				custom_image->SetBrushFromTexture(poi_comp->GetIconImage());
				default_image->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UPointOfInterestWidget::SetOwner(AActor* _Owner)
{
	owner = _Owner;

	AGCharacter* owner_character = Cast<AGCharacter>(owner);

	if (IsValid(owner_character))
	{
		owner_name = owner_character->GetCharacterName().ToString();
	}
	else
	{
		owner_name = "None";
	}
}
