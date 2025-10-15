// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DesertSandElemental.h"
#include "AIController_DesertSandElemental.h"

ANPC_DesertSandElemental::ANPC_DesertSandElemental()
{
	AIControllerClass = AAIController_DesertSandElemental::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
