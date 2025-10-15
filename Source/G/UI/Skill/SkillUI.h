// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/GameMenuBase.h"
#include "G/Util/Define.h"
#include "SkillUI.generated.h"

class USkillCompanionToggleButton;
class USkillTreePanel;
class USkillTreeNodeTooltip;
class APlayerCharacter;

class UVerticalBox;
class UOverlay;

/**
 * 
 */
UCLASS()
class G_API USkillUI : public UGameMenuBase
{
	GENERATED_BODY()

	/* variable */
	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillCompanionToggleButton> bp_skill_toggle_button;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* companion_toggle_box;

	UPROPERTY(meta = (BindWidget))
	UOverlay* skill_tree_overlay;

	UPROPERTY()
	USkillTreePanel* current_skill_tree_panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillTreePanel> bp_skill_tree_panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillTreeNodeTooltip> bp_tooltip;

	UPROPERTY()
	USkillTreeNodeTooltip* tooltip;

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	/* function */
private:

public:
	virtual void On() override;
	virtual void Off() override;

	void Update();
	void Init();
	void ClearCTB() const;
	
	void AddCompanionToggleButton(APlayerCharacter* character);
	USkillCompanionToggleButton* GetCTB(ECharacterName name);
};
