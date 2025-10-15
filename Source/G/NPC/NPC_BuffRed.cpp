// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/NPC_BuffRed.h"
#include "G/NPC/AIController_BuffRed.h"

ANPC_BuffRed::ANPC_BuffRed()
{
	AIControllerClass = AAIController_BuffRed::StaticClass();
}
