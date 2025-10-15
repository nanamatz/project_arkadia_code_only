// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "BuffDebuffImage.generated.h"

/**
 * 
 */
UCLASS()
class G_API UBuffDebuffImage : public UImage
{
	GENERATED_BODY()

public:
	int32* turn_count_ptr;

	bool ProcessTurn(TArray<class UBuffDebuffImage*>& buff_debuff_arr, struct FAnchors& buff_debuff_anchors);
};
