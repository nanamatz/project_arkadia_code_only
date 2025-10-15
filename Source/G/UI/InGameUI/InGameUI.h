// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Util/Define.h"
#include "InGameUI.generated.h"

class UImage;
class UProgressBar;
class UHorizontalBox;
class UCanvasPanel;
class UTexture2D;
class UVerticalBox;
class UButton;
class UMenuToggleButton;
class UScrollBox;
class USizeBox;
class UAudioComponent;
class UTextBlock;

class UTurnQueueWidget;
class USkillButton;
class UPlayerPortrait;
class UCircularPlayerPortrait;
class UAnnotationWidget;
class UMapWidget;
class USkillButtonTooltip;
class USkillSlot;
class APlayerCharacter;
class AGCharacter;
class UESCMenuWidget;
class UInteractionCharacterInfoUI;

/**
 * 
 */

struct portrait
{
	//class UCanvasPanel* portrait;
	UImage* portrait_img;
	UProgressBar* portrait_hp;
	UHorizontalBox* portrait_cost;
};

UCLASS()
class G_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* skill_container;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* skill_box;
	UPROPERTY(meta = (BindWidget))
	UCircularPlayerPortrait* skill_portrait;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* skill_cost;

	UPROPERTY(meta = (BindWidget))
	UButton* skill_cancel_bt;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* portrait_box;

	UPROPERTY(meta = (BindWidget))
	UMapWidget* minimap;

	UPROPERTY(meta = (BindWidget))
	UButton* turn_end_button;

	UPROPERTY(meta = (BindWidget))
	UESCMenuWidget* esc_menu;

	UPROPERTY(meta = (BindWidget))
	UMenuToggleButton* inventory_bt;

	UPROPERTY(meta = (BindWidget))
	UMenuToggleButton* skill_bt;
	
	UPROPERTY(meta = (BindWidget))
	UMenuToggleButton* quest_bt;

	UPROPERTY(meta = (BindWidget))
	UMenuToggleButton* map_bt;

	UPROPERTY(meta = (BindWidget))
	UButton* setting_bt;

	UPROPERTY(meta = (BindWidget))
	UButton* plus_bt;
	
	UPROPERTY(meta = (BindWidget))
	UButton* minus_bt;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* game_speed_text;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* img_cost_bar;

	UPROPERTY(EditAnywhere)
	UTexture2D* normal_skillButton;
	UPROPERTY(EditAnywhere)
	UTexture2D* pressed_skillButton;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* map_data;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillButtonTooltip> bp_skill_button_tooltip;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerPortrait> bp_player_portrait;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillButton> bp_skill_button;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTurnQueueWidget> bp_turn_queue_widget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInteractionCharacterInfoUI> bp_interact_char_info_ui;
	

	UPROPERTY()
	TMap<ECharacterName, UPlayerPortrait*> portrait_arr;

	UPROPERTY()
	USkillButtonTooltip* skill_button_tooltip;

	int32 curr_cost = 0;

	FLinearColor CostColor = FLinearColor(255, 255, 255);
	FLinearColor HighlightColor = FLinearColor(255, 0, 0);
	FLinearColor UsedColor = FLinearColor(0, 0, 0);

	UPROPERTY(VisibleInstanceOnly)
	UTurnQueueWidget* turn_queue_widget;

	UPROPERTY()
	UInteractionCharacterInfoUI* interact_char_info_ui;
	
protected:
	virtual void NativeConstruct() override;

	/* function */
private:
	void CreateTurnQueue();

	void AddCostBar(int32 value);
	void RemoveCostBar(int32 value);
	void UseCostBar(int32 value);
	/* UpdateCost should be called when the player's cost information needs to be synchronized to the UI.  */
	void UpdateCost(AGCharacter* character);

	void CreateInteractCharInfoUI();
	USkillButton* AddSkillButton(int32 skill_num);
	void AddSkillButton(int32 first_skill_num, int32 cnt);
	
	UFUNCTION()
	void CancelSkill();

	UFUNCTION()
	void IncreaseGameSpeed();

	UFUNCTION()
	void DecreaseGameSpeed();
	
protected:
	virtual bool Initialize() override;
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<USkillButton*> button_array;

	void Init();
	void On();
	void Off();

	UFUNCTION()
	void OnClickedTurnEnd();

	void EndTurn();
	void EndCombat();

	UFUNCTION(BlueprintCallable)
	UMapWidget* GetMinimap();
	
	UESCMenuWidget* GetEscMenu();
	UTurnQueueWidget* GetTurnQueueUI();
	UHorizontalBox* GetSkillCost();
	UCircularPlayerPortrait* GetCircularPortrait();
	
	void AddPortrait(APlayerCharacter* pc);
	void DeletePortrait(enum ECharacterName name);
	void DeletePortrait(UPlayerPortrait* pp);
	void ClearPortrait();
	void UpdatePortraitHP(enum ECharacterName name);
	void UpdateHP(enum ECharacterName name);


	/* SetCost must be called when the PlayerTurn is started  */
	void SetCost(AGCharacter* character);
	void IncreaseMaxCost(AGCharacter* character, int32 increasing_cost);
	void DecreaseMaxCost(AGCharacter* character, int32 decreasing_cost);

	void HighlightCostReduction(int32 reduction);
	void UnHighlightCostReduction();

	/*void SetSkillButton(USkillSlot* _skillSlot, int index);*/
	void SetSkillButton(TArray<USkillSlot*> SkillSlotList, bool isPlayerTurn = true);
	void SetSkillIcon(UTexture2D* icon, int index = 4);

	void ActivePortrait(enum ECharacterName name);
	void DeactivePortrait(enum ECharacterName name);

	void StartPlayerTurn(APlayerCharacter* pc);
	void EndPlayerTurn();

	/* skill container */
	void OnSkillContainer() const;
	void OffSkillContainer() const;
	void OnSkillBox() const;
	void OffSkillBox() const;
	void OnCost() const;
	void OffCost() const;

	/* skill box */
	void SetSkillPortrait(APlayerCharacter* pc);
	void UpdateSkillPortrait();
	void UpdateControlCharacter() const;

	/*InteractCharInfoUi*/
	void OnInteractCharInfoUI(FString name, FVector2D pos, FVector2D offset);
	void OffInteractCharInfoUI();
};
