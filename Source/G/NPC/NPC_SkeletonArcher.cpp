// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/NPC_SkeletonArcher.h"
#include <G/NPC/AIController_SkeletonArcher.h>

ANPC_SkeletonArcher::ANPC_SkeletonArcher()
{
	AIControllerClass = AAIController_SkeletonArcher::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
