// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionCharacterInfoActor.h"
#include "InteractionCharacterInfoUI.h"
#include "Components/WidgetComponent.h"

// Sets default values
AInteractionCharacterInfoActor::AInteractionCharacterInfoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionCharacterInfoWidgetComponent = nullptr;

	static ConstructorHelpers::FClassFinder<UInteractionCharacterInfoUI> WidgetClassFinder(
		TEXT("WidgetBlueprint'/Game/UI/GameUI/BP_InterCharInfoWidget.BP_InterCharInfoWidget_C'"));
	InteractionCharacterInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(
		TEXT("InteractionCharacterInfoWidgetComponent"));
	if (WidgetClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("AInteractionCharacterInfoActor 22 - WidgetClassFinder Succeeded"));
		InteractionCharacterInfoWidgetComponent->SetWidgetClass(WidgetClassFinder.Class);
		InteractionCharacterInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}
	SetRootComponent(InteractionCharacterInfoWidgetComponent);

	UE_LOG(LogTemp, Log, TEXT("AInteractionCharacterInfoActor 27 - Initialze"));
}

// Called when the game starts or when spawned
void AInteractionCharacterInfoActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractionCharacterInfoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Initialization)
	{
		if ((InteractionCharacterInfoWidgetComponent->GetWidget()))
		{
			UE_LOG(LogTemp, Log, TEXT("AInteractionCharacterInfoActor 46 - InteractionCharacterInfoWidgetComponent is vaild"));
		}
		else
			UE_LOG(LogTemp, Log, TEXT("AInteractionCharacterInfoActor 48 - InteractionCharacterInfoWidgetComponent is nullptr"));

		//Cast<UInteractionCharacterInfoUI>(InteractionCharacterInfoWidgetComponent->GetWidget())->Init(CharName);
		Initialization = true;
	}
}


void AInteractionCharacterInfoActor::Init(FVector location, FString Name)
{
	SetActorLocation(location);
	this->CharName = Name;
	Initialization = false;
}

void AInteractionCharacterInfoActor::Active()
{
	Cast<UInteractionCharacterInfoUI>(InteractionCharacterInfoWidgetComponent->GetWidget())->Active();
}

void AInteractionCharacterInfoActor::DeActive()
{
	Cast<UInteractionCharacterInfoUI>(InteractionCharacterInfoWidgetComponent->GetWidget())->DeActive();
}
