// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreePanel.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillTreePanel : public UUserWidget
{
	GENERATED_BODY()

	/* variable */
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* skill_tree;

	UPROPERTY()
	TArray<class USkillTreeUI*> skill_tree_ui_arr;

	UPROPERTY()
	TArray<class USkillSlot*> skillSlotList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USkillTreeUI> bp_skill_tree_ui;

	/* function */
public:
	void Init(class UWidgetTree* _WidgetTree, class APlayerCharacter* character, class USkillTreeNodeTooltip* _tooltip);
	void CreateSkillTree(class USkillTreeNode* node, int tree_idx, class USkillTreeNodeTooltip* _tooltip);
};
