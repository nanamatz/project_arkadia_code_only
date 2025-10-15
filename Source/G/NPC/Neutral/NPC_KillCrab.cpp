// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_KillCrab.h"

#include "AIController_KillCrab.h"

ANPC_KillCrab::ANPC_KillCrab()
{
	AIControllerClass = AAIController_KillCrab::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
