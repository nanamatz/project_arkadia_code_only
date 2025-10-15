// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Level/MainCameraDirector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCameraDirector::AMainCameraDirector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainCameraDirector::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainCameraDirector::Tick(float DeltaTime)
{
	APlayerController* MainCameraController = UGameplayStatics::GetPlayerController(this, 0);
	if (MainCameraController)
	{
		if (MainCameraController->GetViewTarget() != MainCam)
		{
			MainCameraController->SetViewTarget(MainCam);
		}
	}
}
