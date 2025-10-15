// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/System/HorizontalComboBox.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "G/G.h"

bool UHorizontalComboBox::Initialize()
{
	bool res = Super::Initialize();

	if (IsValid(left_button))
	{
		left_button->OnClicked.AddDynamic(this, &UHorizontalComboBox::Prev);
	}

	if (IsValid(right_button))
	{
		right_button->OnClicked.AddDynamic(this, &UHorizontalComboBox::Next);
	}

	if (IsValid(selected_option))
	{
		if (default_options.Num() > 0 && default_options.Num() > selected_idx)
		{
			selected_option->SetText(default_options[selected_idx]);
		}
	}
	
	set_idx = selected_idx;
	
	return res;
}

void UHorizontalComboBox::NativeConstruct()
{
	Super::NativeConstruct();

	selected_idx = set_idx;
	selected_option->SetText(default_options[set_idx]);
}

void UHorizontalComboBox::Prev()
{
	if (selected_idx == 0)
	{
		selected_idx = default_options.Num() - 1;
	}
	else
	{
		selected_idx--;
	}

	selected_option->SetText(default_options[selected_idx]);
}

void UHorizontalComboBox::Next()
{
	selected_idx++;
	selected_idx %= default_options.Num();

	selected_option->SetText(default_options[selected_idx]);
}

FText UHorizontalComboBox::GetSelectedOption()
{
	return default_options[selected_idx];
}

int32 UHorizontalComboBox::GetSelectedOptionIdx()
{
	return selected_idx;
}

void UHorizontalComboBox::SetDefaultOptions(TArray<FText>& option_list)
{
	default_options.Empty();

	for (FText option : option_list)
	{
		default_options.Add(option);
	}
}

void UHorizontalComboBox::AddDefaultOption(FText option)
{
	default_options.Add(option);
}

void UHorizontalComboBox::ClearDefaultOptions()
{
	if (default_options.Num() > 0)
	{
		default_options.Empty();
		selected_idx = 0;
	}
}

void UHorizontalComboBox::SetSelectedOption(FText option)
{
	if(default_options.Num() == 0)
		return;

	selected_idx = 0;
	selected_option->SetText(default_options[selected_idx]);
	
	for (int i = 0; i < default_options.Num(); i++)
	{
		if (option.EqualTo(default_options[i]))
		{
			LOG(Log, "SetSelectedOption using FText: %s", *option.ToString());
			break;
		}

		Next();
	}

	set_idx = selected_idx;
}

void UHorizontalComboBox::SetSelectedOption(int32 idx)
{
	selected_idx = idx;

	selected_option->SetText(default_options[selected_idx]);
	/*for (int i = 0; i < idx; i++)
	{
		Next();
	}*/

	set_idx = selected_idx;
}

void UHorizontalComboBox::Apply()
{
	set_idx = selected_idx;
}