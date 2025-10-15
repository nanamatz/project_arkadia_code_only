// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Player/CardenioClass.h"
#include <G/GGameMode.h>


ACardenioClass::ACardenioClass()
{
}

void ACardenioClass::BeginPlay()
{
	Super::BeginPlay();

	/* Spawn recording Actor for Inventory Character Capture */
	GetWorld()->SpawnActor<AActor>(bp_recording_actor, FVector(bp_recording_actor_x, 0, 0), FRotator(0, 0, 0));
}
