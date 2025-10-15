// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DesertArcher.h"
#include "AIController_DesertArcher.h"

ANPC_DesertArcher::ANPC_DesertArcher()
{
	AIControllerClass = AAIController_DesertArcher::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
