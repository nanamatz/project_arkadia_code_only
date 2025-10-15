#include "NPC_VastiaArcher.h"

#include "AIController_VastiaArcher.h"

ANPC_VastiaArcher::ANPC_VastiaArcher()
{
	AIControllerClass = AAIController_VastiaArcher::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
