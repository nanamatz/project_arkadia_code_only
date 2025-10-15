// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SystemPanelBase.h"
#include "KeySettingPanel.generated.h"

/**
 * 
 */
UCLASS()
class G_API UKeySettingPanel : public USystemPanelBase
{
	GENERATED_BODY()

private:
	void ChangeKeyBinding(FName action_name, FKey key);
	
public:
	virtual void On() override;
	virtual void Off() override;
	virtual void Apply() override;
};
