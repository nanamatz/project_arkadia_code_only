// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Player/SamiraClass.h"

void ASamiraClass::BeginPlay()
{
	Super::BeginPlay();

	/* Spawn recording Actor for Inventory Character Capture */
	GetWorld()->SpawnActor<AActor>(bp_recording_actor, FVector(bp_recording_actor_x + 1500, 0, 0), FRotator(0, 0, 0));
}

ASamiraClass::ASamiraClass()
{
}
