// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterInfoSaveGame.h"

void UPlayerCharacterInfoSaveGame::AddCharacterSkill(ECharacterName characterName, class USkillSlot* skillSlot)
{
	if(!CharacterSkill.Contains(characterName))
	{
		CharacterSkill.Add(characterName);
	}
	CharacterSkill[characterName].AddSkillSaveData(skillSlot);
}
