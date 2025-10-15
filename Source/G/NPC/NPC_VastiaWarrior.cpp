#include "NPC_VastiaWarrior.h"

#include "AIController_VastiaWarrior.h"

ANPC_VastiaWarrior::ANPC_VastiaWarrior()
{
	AIControllerClass = AAIController_VastiaWarrior::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
