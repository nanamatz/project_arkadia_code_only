// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/DialogManageActor.h"
#include "G/Dialog/DialogComponentBase.h"

// Sets default values
ADialogManageActor::ADialogManageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UDialogComponentBase> DialogClassFinder(
		TEXT("Blueprint'/Game/DialogSystem/GDialogComponent.GDialogComponent_C'"));

	if (DialogClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("ADialogManageActor 19 :: DialogObjectFinder is success"));
		DialogComponent = 
			Cast<UDialogComponentBase>(CreateDefaultSubobject(TEXT("DialogComponent"),
				DialogClassFinder.Class, DialogClassFinder.Class, false, false));
	}
}

// Called when the game starts or when spawned
void ADialogManageActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ADialogManageActor 28 :: BeginPlay Execute"));
	if (DialogComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("ADialogManageActor 31 :: DialogComponent is vaild"));
		DialogComponent->Init(this);
		DialogComponent->SetDialogTreeBase(DialogTreeBase);
	}
}

// Called every frame
void ADialogManageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("ADialogManageActor 41 :: Tick Execute"));
}

void ADialogManageActor::init(UBehaviorTree* dialogtreebase, AInteractionCharacter* oic)
{
	UE_LOG(LogTemp, Log, TEXT("ADialogManageActor 46 :: Init is execute"));
	DialogTreeBase = dialogtreebase;
	DialogComponent->SetDialogTreeBase(DialogTreeBase);
	OwnerInterChar = oic;
}

class UDialogComponentBase* ADialogManageActor::GetDialogComponent()
{
	return DialogComponent;
}
