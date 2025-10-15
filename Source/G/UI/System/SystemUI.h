// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/GameMenuBase.h"
#include "SystemUI.generated.h"

class UButton;
class UBorder;
class UGraphicPanel;
class UAudioPanel;
class UControlPanel;
class UKeySettingPanel;
class UGamePlayPanel;
class USystemPanelBase;

/**
 * 
 */
UCLASS()
class G_API USystemUI : public UGameMenuBase
{
	GENERATED_BODY()

	/* variable */

	UPROPERTY()
	USystemPanelBase* current_panel;

	UPROPERTY(meta = (BindWidget))
	UButton* graphic_button;

	UPROPERTY(meta = (BindWidget))
	UButton* audio_button;


	UPROPERTY(meta = (BindWidget))
	UButton* key_button;

	UPROPERTY(meta = (BindWidget))
	UButton* gameplay_button;

	UPROPERTY(meta = (BindWidget))
	UBorder* system_panel_border;

	UPROPERTY(meta = (BindWidget))
	UButton* apply_button;

	UPROPERTY(meta = (BindWidget))
	UButton* exit_bt;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGraphicPanel> bp_graphic_panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAudioPanel> bp_audio_panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKeySettingPanel> bp_key_setting_panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGamePlayPanel> bp_game_play_panel;

	UPROPERTY()
	UGraphicPanel* graphic_panel;

	UPROPERTY()
	UAudioPanel* audio_panel;

	UPROPERTY()
	UKeySettingPanel* key_setting_panel;

	UPROPERTY()
	UGamePlayPanel* game_play_panel;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;

	/* function */
private:
	void Update();

	UFUNCTION()
	void ChangePanel(int32 idx);

	UFUNCTION()
	void OnGraphicPanel();
	UFUNCTION()
	void OnAudioPanel();
	UFUNCTION()
	void OnKeySettingPanel();
	UFUNCTION()
	void OnGamePlayPanel();

public:
	virtual void On() override;
	UFUNCTION()
	virtual void Off() override;

	UGraphicPanel* GetGraphicPanel() const { return graphic_panel; }
};
