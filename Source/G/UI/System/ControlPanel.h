// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemPanelBase.h"
#include "ControlPanel.generated.h"

/**
 * 
 */
UCLASS()
class G_API UControlPanel : public USystemPanelBase
{
	GENERATED_BODY()

public:
	virtual void On() override;
	virtual void Off() override;
};
