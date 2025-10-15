// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/SkillInfoManager.h"
#include <G/Skill/SkillData/SkillStruct.h>

#include "G/G.h"

void USkillInfoManager::SetSkillInfo(FSkillStruct _skillInfo)
{
	skillInfo = _skillInfo;
	for (auto action : skillInfo.ActionClassList)
	{
		UActionBase* newAction = NewObject<UActionBase>(this, action.Get());
		skillInfo.ActionList.Add(newAction);
	}
}

void USkillInfoManager::SetSkillRangeType(ESkillRangeType _type)
{
	if (_type != ESkillRangeType::None)
	{
		skillInfo.SkillRangeType = _type;
	}
}

void USkillInfoManager::SetTargetType(ESkillTargetType type)
{
	if (type != ESkillTargetType::NONE)
	{
		skillInfo.TargetType = type;
	}
}

void USkillInfoManager::SetAttackType(EAttackType type)
{
	for (auto& attack : GetAttackList())
	{
		if (type != EAttackType::None)
		{
			attack.Type = type;
		}
	}
}

void USkillInfoManager::AddCharacterRange(float _addNum)
{
	if (_addNum == 0)
	{
		return;
	}
	skillInfo.CharacterRange += _addNum;
	if (skillInfo.CharacterRange <= 0.0f)
	{
		skillInfo.CharacterRange = 0.0f;
	}
}

void USkillInfoManager::AddMouseRange(float _addNum)
{
	if (_addNum == 0)
	{
		return;
	}
	skillInfo.MouseRange += _addNum;
	if (skillInfo.MouseRange <= 0.0f)
	{
		skillInfo.MouseRange = 0.0f;
	}
	LOG(Log, "Cur MouseRange : %.2f", skillInfo.MouseRange);
}

void USkillInfoManager::AddCurCoolDown(int _num)
{
	if (_num == 0)
	{
		return;
	}
	curCoolDown += _num;
	if (curCoolDown > skillInfo.CoolDown)
	{
		curCoolDown = skillInfo.CoolDown;
	}
	if (curCoolDown < 0)
	{
		curCoolDown = 0;
	}
}

void USkillInfoManager::AddCoolDown(int _num)
{
	if (_num == 0)
	{
		return;
	}
	skillInfo.CoolDown += _num;
	if (skillInfo.CoolDown < 0)
	{
		skillInfo.CoolDown = 0;
	}
}

void USkillInfoManager::AddAttackDamage(int32 value)
{
	if (value == 0)
	{
		return;
	}
	for (auto& attack : GetAttackList())
	{
		LOG(Log, "Damage Up : %d", value);
		attack.MinDamage += value;
		attack.MaxDamage += value;
		if (attack.MinDamage <= 0 && attack.MaxDamage <= 0)
		{
			attack.MinDamage = 0;
			attack.MaxDamage = 0;
		}
		LOG(Log, "Cur Min Damage : %d, Max Damage : %d", attack.MinDamage, attack.MaxDamage);
	}
}

void USkillInfoManager::AddDuration(int32 value)
{
	for (auto& attack : GetAttackList())
	{
		if (attack.Duration == 0)
		{
			continue;
		}
		attack.Duration += value;
		if (attack.Duration <= 0)
		{
			attack.Duration = 0;
		}
	}
}

void USkillInfoManager::AddAttackType(FSkillCastAttack newAttack)
{
	LOG(Log, "New %s Value : %d, Duration : %d", *UEnum::GetValueAsString(newAttack.Type), newAttack.MinDamage,
		newAttack.Duration);

	skillInfo.AttackList.Add(newAttack);
}

void USkillInfoManager::AddBuffType(FSkillCastBuff newBuff)
{
	if (newBuff.Value == 0)
	{
		return;
	}

	for (auto& buff : skillInfo.BuffList)
	{
		if (buff.type == newBuff.type)
		{
			buff.Value += newBuff.Value;
			buff.Duration += newBuff.Duration;
			LOG(Log, "Add %s Upgrede Value : %d, Duration : %d", *UEnum::GetValueAsString(newBuff.type), buff.Value,
			    buff.Duration);

			return;
		}
	}
	LOG(Log, "New %s Value : %d, Duration : %d", *UEnum::GetValueAsString(newBuff.type), newBuff.Value,
	    newBuff.Duration);

	skillInfo.BuffList.Add(newBuff);
}

void USkillInfoManager::AddDebuffType(FSkillCastDebuff newDebuff)
{
	if (newDebuff.Value == 0)
	{
		return;
	}


	for (auto& debuff : skillInfo.DebuffList)
	{
		if (debuff.type == newDebuff.type)
		{
			debuff.Value += newDebuff.Value;
			debuff.Duration += newDebuff.Duration;
			LOG(Log, "Add %s Upgrede Value : %d, Duration : %d", *UEnum::GetValueAsString(newDebuff.type), debuff.Value,
			    debuff.Duration);

			return;
		}
	}
	LOG(Log, "New %s Value : %d, Duration : %d", *UEnum::GetValueAsString(newDebuff.type), newDebuff.Value,
	    newDebuff.Duration);
	skillInfo.DebuffList.Add(newDebuff);
}

void USkillInfoManager::AddActionType(TSubclassOf<UActionBase> newAction)
{
	skillInfo.ActionClassList.Add(newAction);
	UActionBase* action = NewObject<UActionBase>(this, newAction.Get());
	LOG(Log, "Add %s action", *action->GetName());
	skillInfo.ActionList.Add(action);
}

void USkillInfoManager::AddDescription(const FText newDescription)
{
	skillInfo.Description = FText::Format(
		FText::FromString("{0}\n{1}"), 
		skillInfo.Description, 
		newDescription
	);
	LOG(Log, "%s", *skillInfo.Description.ToString());
}
