// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevel.h"

#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/GGameMode.h"

void AMainMenuLevel::BeginPlay()
{
	Super::BeginPlay();


	AGGameMode* GameMode = Cast<AGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (_MainMenuWidget)
	{
		MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(GetWorld(), _MainMenuWidget));
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		}
	}
}
