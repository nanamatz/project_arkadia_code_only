// Fill out your copyright notice in the Description page of Project Settings.


#include "SeeingThroughActor.h"

// Sets default values
ASeeingThroughActor::ASeeingThroughActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SeeingTroughMesh = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("SeeingThroughMesh"));
	RootComponent = SeeingTroughMesh;
	//SeeingTroughMesh->CreateDynamicMaterialInstance(0);
	//SeeingTroughMesh->SetCollisionProfileName(TEXT("InvisibleWall"));

	SeeingThroughTime = 0.3f;
	RunningTimer = 0.0f;
	bSeeingThrough = false;
}

// Called when the game starts or when spawned
void ASeeingThroughActor::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void ASeeingThroughActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTimer += DeltaTime;
	float Opacity = 0.0f;

	if (bSeeingThrough)
	{
		Opacity = FMath::Lerp(1.0f, 0.0f, RunningTimer * (1.0f / SeeingThroughTime));
	}
	else
	{
		Opacity = FMath::Lerp(0.0f, 1.0f, RunningTimer * (1.0f / SeeingThroughTime));
	}
	SeeingTroughMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), Opacity);

	if (RunningTimer > SeeingThroughTime)
	{
		RunningTimer = 0.0f;
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void ASeeingThroughActor::SetShowSeeingThrough(bool bThroughShow)
{
	bSeeingThrough = bThroughShow;
	if (RunningTimer != 0.0f)
	{
		RunningTimer = SeeingThroughTime - RunningTimer;
	}
	PrimaryActorTick.SetTickFunctionEnable(true);
}
