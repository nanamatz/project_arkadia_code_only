// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/GameMenuTooltip.h"
#include "SkillTreeNodeTooltip.generated.h"

/**
 * 
 */
UCLASS()
class G_API USkillTreeNodeTooltip : public UGameMenuTooltip
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* skill_name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* requirement_text;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* strength_requirement;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* insight_requirement;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* swift_requirement;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* relaxed_requirement;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* careful_requirement;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* luck_requirement;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void On(class USkillTreeNode* node_info);
	void Off();
};
