// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/NPC_BuffGreen.h"
#include "G/NPC/AIController_BuffGreen.h"
#include <G/GGameMode.h>
#include "G/CombatArea.h"
#include <Kismet/GameplayStatics.h>

#include "G/Manager/CombatManager.h"

ANPC_BuffGreen::ANPC_BuffGreen()
{
	AIControllerClass = AAIController_BuffGreen::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}