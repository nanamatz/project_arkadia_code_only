// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_VastiaCaptain.h"

#include "AIController_VastiaCaptain.h"

ANPC_VastiaCaptain::ANPC_VastiaCaptain()
{
	AIControllerClass = AAIController_VastiaCaptain::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
