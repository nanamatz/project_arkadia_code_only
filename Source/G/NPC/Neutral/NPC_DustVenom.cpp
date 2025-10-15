// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DustVenom.h"
#include "AIController_DustVenom.h"

ANPC_DustVenom::ANPC_DustVenom()
{
	AIControllerClass = AAIController_DustVenom::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
