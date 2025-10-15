// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Skill/SkillTreeNodeStruct.h"
#include "SkillTreeUI.generated.h"

/**
 * 
 */

UCLASS()
class G_API USkillTreeUI : public UUserWidget
{
	GENERATED_BODY()

	/* variable */
public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* skill_tree;

	/* function */
protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	                          const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	                          const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	void AddNode(class USkillTreeNode* node, int tree_idx, class USkillTreeNodeTooltip* _tooltip);
};
