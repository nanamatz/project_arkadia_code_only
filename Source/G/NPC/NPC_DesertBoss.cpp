// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DesertBoss.h"
#include "AIController_DesertBoss.h"

ANPC_DesertBoss::ANPC_DesertBoss()
{
	AIControllerClass = AAIController_DesertBoss::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
