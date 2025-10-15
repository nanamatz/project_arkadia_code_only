// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystemComponent.h"
#include <G/Skill/SkillBase.h>
#include <G/PlayerCharacter.h>
#include <G/Skill/SkillSlot.h>
#include "Engine/DataTable.h"
#include "G/G.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Save/PlayerCharacterInfoSaveGame.h"
#include "G/Skill/SkillInfoManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USkillSystemComponent::USkillSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USkillSystemComponent::SetSkillSlot(AGCharacter* myCharacter, UDataTable* _skillData, bool isStart)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));

	if(myCharacter->GetECharacterName() == ECharacterName::NonPlayerCharacter)
	{
		LOG(Log, "NPC SkillSet");
		SetStartSkillSlot(myCharacter, _skillData);
	}

	if(GameInstance->GetInGameDataManager()->_loadData && GameInstance->GetInGameDataManager()->_loadData->CharacterSkill.Contains(myCharacter->GetECharacterName()))
	{
		LOG(Log, "Load Skill Slot");
		SetLoadDataSkillSlot(myCharacter);
	}
	// 처음 생성
	else if(isStart && _skillData)
	{
		LOG(Log, "Start Skill Slot");
		SetStartSkillSlot(myCharacter, _skillData);
	}
	// 레벨 이동
	else if(_skillData)
	{
		LOG(Log, "Level Skill Slot");
		SetChangeLevelSkillSlot(myCharacter, _skillData);
	}
}

void USkillSystemComponent::SetStartSkillSlot(AGCharacter* myCharacter, UDataTable* _skillData)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));
	skillSlotList.Reset();
	TArray<FName> RowNames = _skillData->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FSkillStruct* SkillData = _skillData->FindRow<FSkillStruct>(RowName, TEXT("Finding Skill Data"));
		FSkillStruct* newSkill = new FSkillStruct();
		*newSkill = *SkillData;

		if (SkillData)
		{
			FString skillName = newSkill->Name.ToString();
			USkillSlot* newSlot;
			if(myCharacter->GetECharacterName() != ECharacterName::NonPlayerCharacter)
			{
				newSlot = NewObject<USkillSlot>(GameInstance);
			}
			else
			{
				newSlot = NewObject<USkillSlot>(this);	
			}
			
			if(newSlot)
			{
				newSlot->SetSkillSlot(myCharacter, newSkill);
				AddSkillSlot(newSlot);
			}
		}
	}
}

void USkillSystemComponent::SetLoadDataSkillSlot(AGCharacter* myCharacter)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));
	if(GameInstance->GetInGameDataManager()->_loadData && GameInstance->GetInGameDataManager()->_loadData->CharacterSkill.Contains(myCharacter->GetECharacterName()))
	{
		LOG(Log, "Set Load  SkillSlot");
		skillSlotList.Reset();
		FSkillSaveDataList skillDataList = GameInstance->GetInGameDataManager()->_loadData->CharacterSkill[myCharacter->GetECharacterName()];
		for (auto skillData : skillDataList.skillList)
		{
			FSkillStruct* SkillData = new FSkillStruct(skillData.skillInfo);
			FSkillStruct* newSkill = new FSkillStruct();
			*newSkill = *SkillData;

			if (SkillData)
			{
				FString skillName = newSkill->Name.ToString();
				USkillSlot* newSlot = NewObject<USkillSlot>(GameInstance);
				newSlot->SetSkillSlot(myCharacter, newSkill, skillData.skillTree);
				AddSkillSlot(newSlot);
			}
		}
	}
}


void USkillSystemComponent::SetChangeLevelSkillSlot(AGCharacter* myCharacter, UDataTable* _skillData)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));
	skillSlotList.Reset();
	
	TArray<USkillSlot*> playerSkillSlotList = GameInstance->GetInGameDataManager()->GetPlayerSkillSlotList(myCharacter->GetECharacterName());
	for(auto skillSlot : playerSkillSlotList)
	{
		LOG(Log, "AddSkillSlot : %s", *(skillSlot->GetMySkill()->GetSkillInfoManager()->GetSkillName()).ToString());
		if(skillSlot)
		{
			skillSlot->SetPlayerCharacter(myCharacter);
			AddSkillSlot(skillSlot);
		}
	}
}

USkillSlot* USkillSystemComponent::SetSelectedSkill(int SkillNum)
{
	selectedSkillNum = SkillNum;
	if (skillSlotList.IsValidIndex(SkillNum))
	{
		selectedSkillSlot = skillSlotList[SkillNum];
		return selectedSkillSlot;
	}
	return nullptr;
}

TArray<class USkillSlot*> USkillSystemComponent::GetSkillSlotList()
{
	return skillSlotList;
}

class UTexture2D* USkillSystemComponent::GetSkillSlotThumbnail(USkillSlot* _skillSlot)
{
	USkillBase* mySkill = _skillSlot->GetMySkill();
	if (mySkill)
	{
		return mySkill->GetSkillThumnail();
	}
	return nullptr;
}

void USkillSystemComponent::AddSkillSlot(class USkillSlot* _newSlot)
{
	AGCharacter* myCharacter = Cast<AGCharacter>(GetOwner());
	
	if(myCharacter && _newSlot)
	{
		//LOG(Log, "Character : %s, Add Skill Slot : %s", *myCharacter->GetName(), *_newSlot->GetName());
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));
		if (myCharacter->GetCharacterInfoManager()->GetECharacterName() != ECharacterName::NonPlayerCharacter)
		{
			GameInstance->GetInGameDataManager()->AddSkillSlot(myCharacter->GetECharacterName(), _newSlot);
		}
		skillSlotList.Add(_newSlot);
	}
}

void USkillSystemComponent::RemoveSelectedSkillSlot()
{
	skillSlotList.Remove(selectedSkillSlot);
	selectedSkillSlot->Remove();
}

void USkillSystemComponent::RemoveSkillSlot(USkillSlot* SkillSlot)
{
	if (SkillSlot)
	{
		skillSlotList.Remove(SkillSlot);
		SkillSlot->Remove();

		AGCharacter* myCharacter = Cast<AGCharacter>(GetOwner());
		if (myCharacter)
		{
			UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(myCharacter->GetWorld()));
			GameInstance->GetInGameDataManager()->RemoveSkillSlot(myCharacter->GetECharacterName(), SkillSlot);
		}
	}
}

void USkillSystemComponent::CountSkillCoolDown()
{
	for (USkillSlot* skillSlot : skillSlotList)
	{
		if (skillSlot && skillSlot->GetInit())
		{
			skillSlot->CountSkillCoolDown();
		}
		else
		{
			LOG(Log, "skillSlot NULL");
		}
	}
}

void USkillSystemComponent::UseSkill()
{
	if (skillSlotList.IsValidIndex(selectedSkillNum))
	{
		skillSlotList[selectedSkillNum]->UseSkill();
	}
}

void USkillSystemComponent::UseSkill(USkillSlot* _skillSlot)
{
	_skillSlot->UseSkill();
}

bool USkillSystemComponent::CheckSkillPossibility()
{
	if (selectedSkillSlot)
	{
		USkillBase* selectedSkill = selectedSkillSlot->GetMySkill();
		if (selectedSkill)
		{
			return selectedSkill->CheckSkillPossibility();
		}
		return false;
	}
	return false;
}

bool USkillSystemComponent::CheckTargetInAttackRange(AGCharacter* target)
{
	return selectedSkillSlot->GetMySkill()->CheckTargetInAttackRange(target);
}

bool USkillSystemComponent::GetSelectedSkillInfo(struct FSkillStruct& out)
{
	if (selectedSkillSlot)
	{
		out = selectedSkillSlot->GetMySkill()->GetSkillInfo();
		return true;
	}
	return false;
}

// Called when the game starts
void USkillSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

TArray<USkillBase*> USkillSystemComponent::GetSkillList()
{
	return _skillList;
}

TArray<USkillSlot*>& USkillSystemComponent::GetSkillSlotListRef()
{
	return skillSlotList;
}


// Called every frame
void USkillSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillSystemComponent::InitSkillSlot()
{
	for (USkillSlot* slot : skillSlotList)
	{
		slot->InitSkill();
	}
}
