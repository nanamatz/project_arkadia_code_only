// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/NPC_DesertThievesWarrior.h"

#include "AIController_DesThievesWarrior.h"
#include "G/GGameInstance.h"
#include "G/Player/CharacterInfoStruct.h"


ANPC_DesertThievesWarrior::ANPC_DesertThievesWarrior()
{
	AIControllerClass = AAIController_DesThievesWarrior::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
