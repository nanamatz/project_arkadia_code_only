// Fill out your copyright notice in the Description page of Project Settings.


#include "KeySettingPanel.h"

#include "GameFramework/InputSettings.h"

void UKeySettingPanel::ChangeKeyBinding(FName action_name, FKey key)
{
	UInputSettings* input_settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (input_settings)
	{
		TArray<FInputActionKeyMapping> action_mappings;
		input_settings->GetActionMappingByName(action_name, action_mappings);
		for (const FInputActionKeyMapping& Mapping : action_mappings)
		{
			input_settings->RemoveActionMapping(Mapping);
		}

		FInputActionKeyMapping new_mapping;
		new_mapping.ActionName = action_name;
		new_mapping.Key = key;
		input_settings->AddActionMapping(new_mapping);

		input_settings->SaveKeyMappings();
		//FSlateApplication::Get().RefreshInputSettings();
	}
}

void UKeySettingPanel::On()
{
	Super::On();
}

void UKeySettingPanel::Off()
{
	Super::Off();
}

void UKeySettingPanel::Apply()
{
	Super::Apply();
}
