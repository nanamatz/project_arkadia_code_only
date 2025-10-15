// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/Inventory/CollocateWidget.h"

#include "CollocatingSkillButton.h"
#include "Components/Button.h"
#include "G/UI/InGameUI/SkillButton.h"
#include "G/UI/InGameUI/SkillButtonTooltip.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/PlayerCharacter.h"
#include "G/Item/UsableItemBase.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Manager/InventoryManager.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "G/SKill/SkillBase.h"
#include "G/Skill/SkillConsumable.h"
#include "G/UI/InGameUI/AnnotationWidget.h"
#include "Kismet/GameplayStatics.h"

class UGGameInstance;

#define LOCTEXT_NAMESPACE "Arkadia"

void UCollocateWidget::Submit()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (TargetPC)
	{
		FString itemName;
		if(Item->IsA(UUsableItemBase::StaticClass()))
		{
			itemName = UEnum::GetDisplayValueAsText(Cast<UUsableItemBase>(Item)->GetEConsumableItemName()).ToString();
		}else
		{
			return;
		}
		FSkillStruct* SkillData = GameInstance->GetDataManager()->GetDataTable<FSkillStruct>(
			ConsumableItemSkillData, itemName);
		FSkillStruct* newSkill = new FSkillStruct();
		*newSkill = *SkillData;

		if (SkillData)
		{
			USkillSlot* newSlot = NewObject<USkillSlot>(GameInstance);
			newSlot->SetSkillSlot(TargetPC, newSkill);
			newSlot->SetPlayerCharacter(TargetPC);
			TargetPC->GetSkillSystemComponent()->AddSkillSlot(newSlot);
		}
	}

	GameInstance->inventory->DeleteItem(Item);

	RemoveFromViewport();
}

void UCollocateWidget::Cancel()
{
	RemoveFromViewport();
}

void UCollocateWidget::AddSkillButton()
{
	UCollocatingSkillButton* new_button = CreateWidget<UCollocatingSkillButton>(this, BP_SkillButton);

	new_button->Init(SkillButtonTooltip, ButtonArray.Num());

	ButtonArray.Add(new_button);

	UHorizontalBoxSlot* horizontal_box_slot = SkillBox->AddChildToHorizontalBox(new_button);

	horizontal_box_slot->SetSize(ESlateSizeRule::Fill);
	horizontal_box_slot->SetVerticalAlignment(VAlign_Center);
	horizontal_box_slot->SetHorizontalAlignment(HAlign_Center);
	horizontal_box_slot->SetPadding(0);
}

void UCollocateWidget::Init()
{
	SkillButtonTooltip = CreateWidget<USkillButtonTooltip>(this, BP_SkillButtonTooltip);

	SubmitButton->OnClicked.AddDynamic(this, &UCollocateWidget::Submit);
	CancelButton->OnClicked.AddDynamic(this, &UCollocateWidget::Cancel);
}

void UCollocateWidget::On(TArray<USkillSlot*>& SkillSlotList, UUsableItemBase* _Item, APlayerCharacter* _TargetPC)
{
	Item = _Item;
	TargetPC = _TargetPC;

	if (TargetPC)
	{
		LOG(Log, "CollocateWidget On");
		Cast<UGGameInstance>(GetGameInstance())->GetAnnotationWidget()->OnAnnotation(FText::Format(LOCTEXT("CollcatingSkill", "{0} 캐릭터에 다음과 같이 스킬이 할당됩니다."), TargetPC->GetCharacterName()), 3, 20);

		Set(SkillSlotList, _Item);
		
		AddToViewport();
	}
	else
	{
		LOG(Log, "TargetPC is nullptr");
		Off();
	}
}

void UCollocateWidget::Off()
{
	RemoveFromViewport();
}

void UCollocateWidget::Set(TArray<USkillSlot*>& SkillSlotList, UUsableItemBase* _Item)
{
	int button_arr_size = ButtonArray.Num();
	int skill_slot_list_size = SkillSlotList.Num();

	LOG(Log, "bt_arr_size: %d, ss_list_size: %d", button_arr_size, skill_slot_list_size);
	
	if(button_arr_size < skill_slot_list_size)
	{
		for(int i = 0; i < skill_slot_list_size - button_arr_size; i++)
			AddSkillButton();
	}
	if(button_arr_size > skill_slot_list_size)
	{
		ButtonArray.RemoveAt(skill_slot_list_size, button_arr_size - skill_slot_list_size);

		for(int i = button_arr_size; i > skill_slot_list_size; i--)
		{
			SkillBox->RemoveChildAt(i-1);
		}
	}
	LOG(Log, "bt_arr_num:%d ss_list_num: %d", ButtonArray.Num(), SkillSlotList.Num());
	for (int i = 0; i < SkillSlotList.Num(); i++)
	{
		if (SkillSlotList[i] && ButtonArray[i])
		{
			bool isPlayerTurn = false;
			ButtonArray[i]->Set(SkillSlotList[i], isPlayerTurn);
		}
	}

	AddSkillButton();

	CurrentCollocatedButton = ButtonArray[SkillSlotList.Num()];
	CurrentCollocatedButton->SetIcon(_Item->GetIcon());
}

#undef LOCTEXT_NAMESPACE