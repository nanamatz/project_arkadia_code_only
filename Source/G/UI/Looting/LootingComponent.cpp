// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingComponent.h"
#include "G/GGameInstance.h"
#include "G/UI/GameMenuManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
ULootingComponent::ULootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	GameInstance = nullptr;
}


// Called when the game starts
void ULootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}


// Called every frame
void ULootingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULootingComponent::OpenLootingUI(FVector2D pos)
{
	if (!GameInstance)
	{
		GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
	GameInstance->GetGameMenuManager()->SetLootingUI(this);
	GameInstance->GetGameMenuManager()->OnLootingUI(pos);
}

void ULootingComponent::CloseLootingUI()
{
	if (!GameInstance)
	{
		GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
	GameInstance->GetGameMenuManager()->OffLootingUI();
}
