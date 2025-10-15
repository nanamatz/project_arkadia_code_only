#include "NPC_Nicolas.h"

#include "AIController_Nicolas.h"

ANPC_Nicolas::ANPC_Nicolas()
{
	AIControllerClass = AAIController_Nicolas::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
