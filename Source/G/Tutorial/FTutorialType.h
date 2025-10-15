#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "G/Util/Define.h"
#include "FTutorialType.generated.h"

USTRUCT(BlueprintType)
struct G_API FTutorialType : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ETutorial Type;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> tutorialWidget;
};
