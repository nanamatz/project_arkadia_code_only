// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfirmationBox.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UConfirmationBox::Confirm()
{
	res = true;

	// 델리게이트 호출 (외부 클래스에서 처리할 수 있도록)
	OnConfirmResult.Broadcast(true);
	
	RemoveFromViewport();
}

void UConfirmationBox::Cancel()
{
	res = false;

	// 델리게이트 호출 (외부 클래스에서 처리할 수 있도록)
	OnConfirmResult.Broadcast(false);
	
	RemoveFromViewport();
}

bool UConfirmationBox::Initialize()
{
	bool _res = Super::Initialize();

	if(confirm_bt)
		confirm_bt->OnClicked.AddDynamic(this, &UConfirmationBox::Confirm);

	if(cancel_bt)
		cancel_bt->OnClicked.AddDynamic(this, &UConfirmationBox::Cancel);
	
	return _res;
}

bool* UConfirmationBox::On(FText _text, bool bCancel)
{
	text->SetText(_text);

	cancel_bt->SetIsEnabled(bCancel);
	
	AddToViewport();

	return &res;
}
