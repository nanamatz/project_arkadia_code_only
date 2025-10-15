// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SkillSlot.h"
#include <G/Skill/SkillBase.h>
#include <G/Skill/SkillTree.h>
#include <G/Skill/SkillData/SkillStruct.h>
#include <G/Skill/SkillTreeNodeStruct.h>
#include <G/PlayerCharacter.h>

#include "SkillInfoManager.h"
#include "G/G.h"
#include "G/UI/InGameUI/SkillButton.h"

void USkillSlot::InitSkill()
{
	mySkill->InitSkill();
}

void USkillSlot::Init(TSubclassOf<USkillBase> _Skill, AGCharacter* _Character)
{
	mySkill = NewObject<USkillBase>(this, _Skill);
	mySkill->SetSkill(_Character);
	_myCharacter = _Character;

	_init = true;
}

void USkillSlot::Init(USkillBase* _Skill, AGCharacter* _Character)
{
	mySkill = _Skill;
	_myCharacter = _Character;

	_init = true;
}

void USkillSlot::Remove()
{
	LOG(Log, "Remove");
	skill_button->Clear();
	RemoveFromRoot();
}

void USkillSlot::SetPlayerCharacter(AGCharacter* myCharacter)
{
	_myCharacter = myCharacter;
	GetMySkill()->SetPlayerCharacter(_myCharacter);
	auto skillTree = GetMySkillTree();
	if(skillTree)
	{
		skillTree->SetPlayerCharacter(_myCharacter);
	}
}

void USkillSlot::SetSkillSlot(class AGCharacter* myCharacter, struct FSkillStruct* _skillData)
{
	_myCharacter = myCharacter;
	mySkill = NewObject<USkillBase>(this, _skillData->skillClass.Get());

	mySkill->SetSkill(_myCharacter, _skillData);
	mySkillTree = NewObject<USkillTree>(this);
	mySkillTree->SetSkillTree(_myCharacter, _skillData->skillTreeData, this);
	_init = true;
}

void USkillSlot::SetSkillSlot(AGCharacter* myCharacter, FSkillStruct* _skillData,
	const TArray<FSkillTreeNodeStruct>& skillTreeNodeList)
{
	_myCharacter = myCharacter;
	mySkill = NewObject<USkillBase>(this, _skillData->skillClass.Get());

	mySkill->SetSkill(_myCharacter, _skillData);
	mySkillTree = NewObject<USkillTree>(this);
	mySkillTree->SetSkillTree(_myCharacter, skillTreeNodeList, this);
	_init = true;
}

void USkillSlot::UseSkill()
{
	LOG(Log, "%s가 %s 사용", *_myCharacter->GetCharacterInfoManager()->GetCharacterName().ToString(),
	    *mySkill->GetSkillInfoManager()->GetSkillName().ToString());
	mySkill->SkillStart();
	if (skill_button)
	{
		skill_button->SetCoolDown();
	}
}

UTexture2D* USkillSlot::GetThumnail()
{
	return mySkill->GetSkillThumnail();
}

FSkillStruct& USkillSlot::GetSkillInfo()
{
	return mySkill->GetSkillInfo();
}

void USkillSlot::CountSkillCoolDown()
{
	if (mySkill)
	{
		mySkill->CountCurCoolDown();

		if (skill_button)
		{
			skill_button->SetCoolDown();
		}
	}
}

void USkillSlot::SetSkillButton(USkillButton* _skill_button)
{
	skill_button = _skill_button;
}

void USkillSlot::SetThumnail(UTexture2D* Thumnail)
{
	mySkill->SetThumnail(Thumnail);
}

class USkillBase* USkillSlot::GetMySkill()
{
	return mySkill;
}

class USkillTree* USkillSlot::GetMySkillTree()
{
	return mySkillTree;
}

TArray<USkillTreeNode*> USkillSlot::GetAllSkillTreeNodeList()
{
	return mySkillTree->GetAllSkillTreeNodeList();
}
