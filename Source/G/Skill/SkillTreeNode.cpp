// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SkillTreeNode.h"

#include "SkillBase.h"
#include "SkillTreeNodeStruct.h"
#include "SkillInfoManager.h"
#include "G/CharacterInfoComponent.h"
#include "G/G.h"
#include "G/GCharacter.h"
#include "G/GGameInstance.h"
#include "G/UI/GameMenuManager.h"
#include "G/UI/InGameUI/AnnotationWidget.h"
#include "G/UI/Inventory/InventoryUI.h"
#include "G/UI/Skill/SkillCompanionToggleButton.h"
#include "G/UI/Skill/SkillUI.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void USkillTreeNode::SetMyCharacter(AGCharacter* myCharacter)
{
	_myCharacter = myCharacter;
}

void USkillTreeNode::SetNodeInfo(FSkillTreeNodeStruct nodeInfo, USkillBase* mySkill, AGCharacter* myCharacter)
{
	LOG(Log, "%s : %d", *nodeInfo.NodeName.ToString(), nodeInfo.IsLearning);
	_nodeInfo = nodeInfo;
	_mySkill = mySkill;
	_myCharacter = myCharacter;
}

void USkillTreeNode::SetIsRequirementMet(int num, bool value)
{
	_nodeInfo.isRequirementMet[num] = value;
}

void USkillTreeNode::ActiveNode()
{
	_nodeInfo.IsLearning = true;
	for (auto node : _childNodeList)
	{
		node->SetIsRequirementMet(_nodeInfo.Num, true);
	}
}

void USkillTreeNode::ClickTreeNode()
{
	ActiveNode();
	USkillBase* mySkill = GetMySkill();
	FUpgradeNodeStruct& upgradeInfo = _nodeInfo.UpgradeSkillInfo;
	
	USkillInfoManager* skillInfoManager = mySkill->GetSkillInfoManager();
	LOG(Log, "%s Upgrade!", *mySkill->GetSkillInfoManager()->GetSkillName().ToString());

	if(upgradeInfo.skillAnimMontage)
	{
		skillInfoManager->SetSkillAnimMontage(upgradeInfo.skillAnimMontage);
	}
	
	skillInfoManager->SetTargetType(upgradeInfo.SkillTargetType);
	skillInfoManager->AddMouseRange(upgradeInfo.MouseRange);
	skillInfoManager->AddCharacterRange(upgradeInfo.CharacterRange);
	skillInfoManager->SetSkillRangeType(upgradeInfo.SkillRangeType);
	skillInfoManager->SetAttackType(upgradeInfo.ChangeAttackType);
	skillInfoManager->AddCost(-upgradeInfo.Cost);
	skillInfoManager->AddCoolDown(-upgradeInfo.CoolDown);
	skillInfoManager->AddAttackDamage(upgradeInfo.UpgradeDamage);
	skillInfoManager->AddDuration(upgradeInfo.UpgradeDuration);
	skillInfoManager->AddDescription(_nodeInfo.Description);
	skillInfoManager->AddSkillCriticalRate(upgradeInfo.AddSkillCriticalRate);
	for (FSkillCastAttack attck : upgradeInfo.AttackList)
	{
		skillInfoManager->AddAttackType(attck);
	}
	for (FSkillCastBuff buff : upgradeInfo.BuffList)
	{
		skillInfoManager->AddBuffType(buff);
	}
	for (FSkillCastDebuff debuff : upgradeInfo.DebuffList)
	{
		skillInfoManager->AddDebuffType(debuff);
	}
	for (auto action : upgradeInfo.ActionList)
	{
		skillInfoManager->AddActionType(action);
	}
}


bool USkillTreeNode::CheckLearning()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	for (auto index : GetSkillTreeNodeRequirement())
	{
		if (!GetIsRequirementMet()[index])
		{
			GameInstance->GetAnnotationWidget()->OnAnnotation(FText(LOCTEXT("SkillNode", "기술 습득을 위한 조건이 충족되지 않았습니다.")),3 );

			return false;
		}
	}
	
	if (_myCharacter->GetCharacterInfoManager()->GetAbilityRef() < GetAbilityRequirement())
	{
		GameInstance->GetAnnotationWidget()->OnAnnotation(FText(LOCTEXT("SkillNodeNeedStat", "캐릭터 능력치가 부족합니다.")), 3);
		
		return false;
	}
	
	if (_myCharacter->GetCharacterInfoManager()->GetSkillPoint() < GetSkillPointRequirement())
	{
		GameInstance->GetAnnotationWidget()->OnAnnotation(FText::Format(LOCTEXT("SkillNodeNeedPoint", "{0} 기술 점수가 부족합니다."), GetSkillPointRequirement() - _myCharacter->GetCharacterInfoManager()->GetSkillPoint()), 3);
		
		return false;
	}
		
	int32 res_sp = _myCharacter->GetCharacterInfoManager()->AddSkillPoint(-GetSkillPointRequirement());

	USkillCompanionToggleButton* ctb =GameInstance-> GetGameMenuManager()->GetSkillUI()->GetCTB(_myCharacter->GetECharacterName());

	if(ctb)
		ctb->SetSkillPoint(res_sp);
	
	return true;
}

#undef LOCTEXT_NAMESPACE