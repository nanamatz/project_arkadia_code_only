#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameLevelPalace.generated.h"

/**
 * 
 */
UCLASS()
class G_API AGameLevelPalace : public ALevelScriptActor
{
	GENERATED_BODY()


protected:
	AGameLevelPalace();
	virtual void BeginPlay() override;
};