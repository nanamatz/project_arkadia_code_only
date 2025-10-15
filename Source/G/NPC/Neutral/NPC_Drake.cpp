// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Drake.h"

#include "AIController_Drake.h"

ANPC_Drake::ANPC_Drake()
{
	AIControllerClass = AAIController_Drake::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
