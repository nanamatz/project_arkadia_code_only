// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuManager.generated.h"

class UInventoryUI;
class USkillUI;
class UQuestUI;
class UMapUI;
class USystemUI;
class UEnhenceUI;
class UGameMenuBase;
class APlayerCharacter;
class UCharacterCaptureWidget;
class ULootingUI;
class UInteractionIcon;

class UButton;

UENUM()
enum class UI_TYPE
{
	None, Inventory, Skill, Quest, /*Upgrade,*/ Map, /*System*/
};

/**
 * 
 */
UCLASS()
class G_API UGameMenuManager : public UUserWidget
{
	GENERATED_BODY()
	/*
* *_button	: BP 상 Button Widget에 Bind하기 위한 변수
* _*		: BP에서 Sub UI Widget BP를 넣어주기 위한 변수
* *			: 해당 변수에 UI를 Sub UI Widget BP를 Create
*/

	/* inventory */
	UPROPERTY(meta = (BindWidget))
	UButton* inventory_button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryUI> bp_inventory;

	/* skill */
	UPROPERTY(meta = (BindWidget))
	UButton* skill_button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillUI> bp_skill;

	/* quest*/
	UPROPERTY(meta = (BindWidget))
	UButton* quest_button;

	UPROPERTY(meta = (BindWidget))
	UButton* exit_bt;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestUI> bp_quest;

	/* map */
	UPROPERTY(meta = (BindWidget))
	UButton* map_button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMapUI> bp_map;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UEnhenceUI> bp_enhence;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULootingUI> bp_looting;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInteractionIcon> bp_intericon;
	
	UPROPERTY(VisibleInstanceOnly)
	UInventoryUI* inventory;

	UPROPERTY(VisibleInstanceOnly)
	USkillUI* skill;

	UPROPERTY(VisibleInstanceOnly)
	UQuestUI* quest;

	UPROPERTY(VisibleInstanceOnly)
	UEnhenceUI* enhence;

	UPROPERTY(VisibleInstanceOnly)
	ULootingUI* looting;
	
	UPROPERTY(VisibleInstanceOnly)
	UInteractionIcon* intericon;

	UPROPERTY(VisibleInstanceOnly)
	UMapUI* map;

	// 현재 켜져있는 UI
	UI_TYPE current_ui;

	UPROPERTY()
	UGameMenuBase* _current_ui;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	/* function */
private:
	FKey SubUIKey();

protected:
	virtual bool Initialize() override;
	
public:
	void Init();
	void Update(APlayerCharacter* player_character) const;
	void Clear() const;

	void On(UI_TYPE UI);

	/* call in blueprint to add event */
	UFUNCTION(BlueprintCallable)
	void Off();

	void OffCurrentUI();
	void CreateSubUI(int32& current_weight, int32& max_weight);

	UFUNCTION()
	void ChangeSubUI(enum UI_TYPE ui);

	void ChangeSubUI(const struct FKey& key);

	UFUNCTION()
	void ChangeToInventory();

	UFUNCTION()
	void ChangeToSkill();

	UFUNCTION()
	void ChangeToQuest();

	UFUNCTION()
	void ChangeToMap();

	UInventoryUI* GetInventoryUI();
	USkillUI* GetSkillUI();
	UQuestUI* GetQuestUI();

	UFUNCTION(BlueprintCallable)
	UMapUI* GetMapUI();
	
	UEnhenceUI* GetEnhenceUI();
	UInteractionIcon* GetInteractionIcon();

	UI_TYPE get_current_ui() const { return current_ui; };

	void AddCharacterCaptureWidget(UCharacterCaptureWidget* ccw);

	void OnEnhenceUI();
	void OffEnhenceUI();

	void OnLootingUI(FVector2D pos);
	void OffLootingUI();
	void SetLootingUI(class ULootingComponent* ltc);
};
