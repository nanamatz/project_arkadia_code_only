// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Guardian.h"
#include "AIController_Guardian.h"

ANPC_Guardian::ANPC_Guardian()
{
	AIControllerClass = AAIController_Guardian::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
