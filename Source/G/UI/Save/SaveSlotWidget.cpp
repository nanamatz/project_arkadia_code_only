// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSlotWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "G/G.h"

void USaveSlotWidget::SetSelectedSlot()
{
	LOG(Log, "save_name_text_ptr");
	
	if(save_name_text_ptr)
	{
		LOG(Log, "save_name_text_ptr");
		(*save_name_text_ptr)->SetText(slot_text->GetText());
	}
}

bool USaveSlotWidget::Initialize()
{
	bool res = Super::Initialize();

	slot_bt->OnClicked.AddDynamic(this, &USaveSlotWidget::SetSelectedSlot);
	
	return res;
}

void USaveSlotWidget::Set(UEditableText** _save_name_text_ptr, FString& path, FDateTime& modified_date)
{
	save_name_text_ptr = _save_name_text_ptr;
	
	slot_text->SetText(FText::FromString(path));
	date_text->SetText(FText::FromString(modified_date.ToString()));
}
