// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/OrderedButton.h"

UButton*& UOrderedButton::GetButton()
{
	return button;
}

void UOrderedButton::SetOrder(int32 _order)
{
	order = _order;
}

int32 UOrderedButton::GetOrder()
{
	return order;
}
