// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/NPCAIController.h"
#include "AIController_DesertBoss.generated.h"

/**
 * 
 */
UCLASS()
class G_API AAIController_DesertBoss : public ANPCAIController
{
	GENERATED_BODY()

	AAIController_DesertBoss();

public:
	static const FName SpawnSandElemental;

	virtual void OnPossess(APawn* InPawn) override;
};
