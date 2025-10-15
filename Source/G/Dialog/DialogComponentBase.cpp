// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/DialogComponentBase.h"
#include "G/Dialog/DialogWidgetBase.h"

// Sets default values for this component's properties
UDialogComponentBase::UDialogComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//bWantsInitializeComponent = true;
	// ...
}

void UDialogComponentBase::DialogWidgetBaseSetOwnerActor()
{
	DialogWidgetBase->OwnerActor = OwnerActor;
}