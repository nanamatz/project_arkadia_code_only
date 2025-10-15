// Fill out your copyright notice in the Description page of Project Settings.


#include "G/NPC/NPC_BuffBlue.h"
#include "G/GGameInstance.h"
#include "G/Player/CharacterInfoStruct.h"
#include "G/NPC/AIController_BuffBlue.h"

ANPC_BuffBlue::ANPC_BuffBlue()
{
	AIControllerClass = AAIController_BuffBlue::StaticClass();
}
