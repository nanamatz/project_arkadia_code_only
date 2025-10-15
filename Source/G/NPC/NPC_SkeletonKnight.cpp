// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/NPC_SkeletonKnight.h"
#include <G/NPC/AIController_SkeletonKnight.h>

ANPC_SkeletonKnight::ANPC_SkeletonKnight()
{
	AIControllerClass = AAIController_SkeletonKnight::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
