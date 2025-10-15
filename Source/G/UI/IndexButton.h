// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "IndexButton.generated.h"

/**
 * 
 */
UCLASS()
class G_API UIndexButton : public UButton
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	int32 index = 0;

public:
	void SetIndex(int32 idx);
};
