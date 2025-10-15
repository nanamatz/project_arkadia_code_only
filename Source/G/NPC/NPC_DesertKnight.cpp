// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DesertKnight.h"
#include "AIController_DesertKnight.h"

ANPC_DesertKnight::ANPC_DesertKnight()
{
	AIControllerClass = AAIController_DesertKnight::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
