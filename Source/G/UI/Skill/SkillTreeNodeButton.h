// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "G/Skill/SkillTreeNode.h"
#include "G/Skill/SkillTreeNodeStruct.h"
#include "SkillTreeNodeButton.generated.h"

/**
 * 
 */

UCLASS()
class G_API USkillTreeNodeButton : public UButton
{
	GENERATED_BODY()

	UPROPERTY()
	class USkillTreeNode* skillTreeNode;

	UPROPERTY()
	class USkillTreeNodeTooltip* tooltip;

	/* function */
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnTooltip();

	UFUNCTION()
	void OffTooltip();

public:
	void Init(class USkillTreeNode* node, class USkillTreeNodeTooltip* _tooltip);

	UFUNCTION(BlueprintCallable)
	bool LearnSkillNode();

	FSkillTreeNodeStruct& GetNodeInfo();
};
