// Fill out your copyright notice in the Description page of Project Settings.


#include "G/CharacterInfoComponent.h"
#include <G/Player/CharacterInfoStruct.h>
#include "G/Character/CharacterInfo.h"

#include "G.h"
#include "GCharacter.h"
#include "GGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GameMenuManager.h"
#include "Manager/CombatManager.h"
#include "Manager/FXManager.h"
#include "UI/GameMenuManager.h"
#include "UI/InGameUI/InGameUI.h"
#include "UI/Inventory/InventoryUI.h"

void UCharacterInfoComponent::InitCharacterInfo()
{
	//characterInfo.CurHP = characterInfo.MaxHP;
	characterInfo.CurCost = characterInfo.MaxCost;
	characterInfo.MoveCost = 0.0f;
	characterInfo.CurShield = characterInfo.DefaultShield;
	characterInfo.BDeath = false;
	characterInfo.BuffList.Reset();
	characterInfo.DeBuffList.Reset();
}

void UCharacterInfoComponent::SetCharacterInfo(FCharacterInfo info, bool bStart)
{
	characterInfo = info;
	if(bStart)
	{
		characterInfo.CurHP = characterInfo.MaxHP;
		characterInfo.CurCost = characterInfo.MaxCost;
		characterInfo.MoveCost = 0.0f;
		characterInfo.cur_exp = 0;
		characterInfo.CurShield = characterInfo.DefaultShield;
		characterInfo.BDeath = false;
		characterInfo.BuffList.Reset();
		characterInfo.DeBuffList.Reset();
	}

	if(characterInfo.CurHP == 0)
	{
		characterInfo.BDeath = false;
		Cast<AGCharacter>(GetOwner())->Death();
	}
}

int32 UCharacterInfoComponent::GetDamage()
{
	int32 abilityValue = 0;
	if (characterInfo.BonusValue > 0)
	{
		abilityValue = (characterInfo.Ability.Value[characterInfo.ImportantAbility] / characterInfo.BonusValue);
	}
	//characterInfo.equipped_item_map[EEquipType::weapon]->
	int32 weaponDamage = 0;
	return abilityValue + weaponDamage;
}

void UCharacterInfoComponent::CountBuffAndDeBuff()
{
	for (int i = 0; i < characterInfo.BuffList.Num(); i++)
	{
		characterInfo.BuffList[i].turnCount--;
	}

	characterInfo.BuffList.RemoveAll([](FBUFF Val)
	{
		return Val.turnCount <= 1;
	});

	for (int i = 0; i < characterInfo.DeBuffList.Num(); i++)
	{
		characterInfo.DeBuffList[i].turnCount--;
	}

	characterInfo.DeBuffList.RemoveAll([](FDEBUFF Val)
	{
		return Val.turnCount <= 1;
	});
}

void UCharacterInfoComponent::LevelUp()
{
	if(GetBDeath())
	{
		return;
	}
	
	characterInfo.Level++;
	float intermediateValue = (148 * characterInfo.Level) - 89.0f;
	characterInfo.max_exp = FMath::RoundToInt(intermediateValue / 10.0f) * 10;

	LOG(Log, "%s LevelUp", *characterInfo.Name.ToString());
	AddSkillPoint(1);
	AddMaxHP(characterInfo.LevelUpInfo.LevelHP);
	AddMaxCost(characterInfo.LevelUpInfo.LevelCost);
	AddCurrentHP(characterInfo.MaxHP);
	
	AddMaxCost(characterInfo.LevelUpInfo.LevelCost);
	AddCurrentCost(characterInfo.MaxCost);
	
	AddMovePerCost(characterInfo.LevelUpInfo.LevelMovePerCost);
	AddAvoidRate(characterInfo.LevelUpInfo.LevelAvoidRate);
	AddHitRate(characterInfo.LevelUpInfo.LevelHitRate);
	AddCriticalRate(characterInfo.LevelUpInfo.LevelCriticalRate);
	AddCriticalDamage(characterInfo.LevelUpInfo.LevelCriticalDamage);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetGameMenuManager()->GetInventoryUI()->SetLevel(characterInfo.Level);

	if(GetWorld())
	{
		AGCharacter* myCharacter = Cast<AGCharacter>(GetOwner());

		GameInstance->GetFXManager()->PlayVFX(EVFXType::LevelUp, myCharacter->GetActorLocation());
		GameInstance->GetInGameUI()->UpdateHP(characterInfo.EName);
		GameInstance->GetInGameUI()->SetCost(myCharacter);
		GameInstance->GetGameMenuManager()->GetInventoryUI()->SetLevel(characterInfo.Level);
	}
}

/*int32 UCharacterInfoManager::CalculateAbility()
{
	
}*/

template <typename T>
void UCharacterInfoComponent::AddValue(T& value, T& addValue, T maxValue, T minValue)
{
	value += addValue;
	if (minValue > value)
	{
		value = minValue;
	}
	if (maxValue < value)
	{
		value = maxValue;
	}
}

void UCharacterInfoComponent::AddCurrentExp(int32 value)
{
	LOG(Log, "%s +%d Exp", *characterInfo.Name.ToString(), value);
	characterInfo.cur_exp += value;

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	while (characterInfo.max_exp < characterInfo.cur_exp)
	{
		if (characterInfo.Level >= characterInfo.MaxLevel)
		{
			characterInfo.cur_exp = characterInfo.max_exp;
			break;
		}
		characterInfo.cur_exp -= characterInfo.max_exp;
		LevelUp();
	}
	GameInstance->GetGameMenuManager()->GetInventoryUI()->SetExp(GetExpPercent());
}

void UCharacterInfoComponent::AddBuffList(struct FBUFF newBuff)
{
	characterInfo.BuffList.Add(newBuff);
}


void UCharacterInfoComponent::AddDeBuffList(struct FDEBUFF newDeBuff)
{
	characterInfo.DeBuffList.Add(newDeBuff);
}
