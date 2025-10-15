// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/UI/InGameUI/InGameUI.h"
#include "SkillSlot.generated.h"

class USkillButton;
class USkillBase;
class USkillTree;
class USkillTreeNode;
class AGCharacter;

/**
 * 
 */
UCLASS()
class G_API USkillSlot : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	USkillButton* skill_button;

protected:
	bool _init = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillBase* mySkill;
	UPROPERTY()
	USkillTree* mySkillTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGCharacter* _myCharacter;

public:
	void InitSkill();
	void Init(TSubclassOf<USkillBase> _Skill, AGCharacter* _Character);
	void Init(class USkillBase* _Skill, AGCharacter* _Character);
	void Remove();

	void SetPlayerCharacter(AGCharacter* myCharacter);
	void SetSkillSlot(AGCharacter* myCharacter, struct FSkillStruct* _skillData);
	void SetSkillSlot(AGCharacter* myCharacter, struct FSkillStruct* _skillData, const TArray<struct FSkillTreeNodeStruct>& skillTreeNodeList);
	
	void SetSkillButton(USkillButton* _skill_button);
	void SetThumnail(UTexture2D* Thumnail);

	bool GetInit() { return _init; }
	USkillBase* GetMySkill();
	UTexture2D* GetThumnail();
	struct FSkillStruct& GetSkillInfo();
	USkillTree* GetMySkillTree();
	TArray<USkillTreeNode*> GetAllSkillTreeNodeList();

	void CountSkillCoolDown();

	void UseSkill();
};
