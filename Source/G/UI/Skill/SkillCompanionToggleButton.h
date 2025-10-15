// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/OrderedButton.h"
#include "G/Util/Define.h"
#include "SkillCompanionToggleButton.generated.h"

class USkillTreePanel;
class USkillTreeNodeTooltip;
class APlayerCharacter;

class UOverlay;
class UTextBlock;

/**
 * 
 */
UCLASS()
class G_API USkillCompanionToggleButton : public UOrderedButton
{
	GENERATED_BODY()
	USkillTreePanel** current_skill_tree_panel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillTreePanel> bp_skill_tree_panel;

	ECharacterName target_character_name;

	int32 skill_point;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* skill_point_text;

public:
	UPROPERTY()
	USkillTreePanel* skill_tree_panel;

	/* function */
private:
	UFUNCTION()
	void ToggleSkillTreeUI();

public:
	void Init(UOverlay* skill_tree_overlay, USkillTreePanel** _current_skill_tree_panel,
	          APlayerCharacter* character, USkillTreeNodeTooltip* _tooltip);
	ECharacterName GetTargetECharacterName();
	void SetSkillPoint(int32 point); 
};
