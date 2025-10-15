// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Player/RuthCindaClass.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"


void ARuthCindaClass::BeginPlay()
{
	Super::BeginPlay();

	/* Spawn recording Actor for Inventory Character Capture */
	GetWorld()->SpawnActor<AActor>(bp_recording_actor, FVector(bp_recording_actor_x + 1000, 0, 0), FRotator(0, 0, 0));
}

ARuthCindaClass::ARuthCindaClass()
{
}
