// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/GameMenuBase.h"

#include "G/GPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UGameMenuBase::On()
{
	AddToViewport();
}

void UGameMenuBase::Off()
{
	RemoveFromViewport();
}
