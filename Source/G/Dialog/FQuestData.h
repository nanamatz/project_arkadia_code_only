// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FQuestData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct G_API FQuestData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	FString Content;
	UPROPERTY(EditAnywhere)
	FString HashTag;
	UPROPERTY(EditAnywhere)
	bool isReady;
};
