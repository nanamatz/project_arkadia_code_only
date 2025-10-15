// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestIconActor.h"
#include "QuestIconUI.h"
#include "G/GPlayerCamera.h"
#include "Components/WidgetComponent.h"
#include "G/Dialog/InteractionCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AQuestIconActor::AQuestIconActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	QuestIconWidgetComponent = nullptr;

	static ConstructorHelpers::FClassFinder<UQuestIconUI> WidgetClassFinder(
		TEXT("WidgetBlueprint'/Game/UI/GameUI/BP_QuestIconUIWidget.BP_QuestIconUIWidget_C'"));
	QuestIconWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestIconWidgetComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectFinder(
		TEXT("StaticMesh'/Game/DialogSystem/JDH_OnlyTestFolder/Exclamation_mark.Exclamation_mark'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectFinder2(
		TEXT("StaticMesh'/Game/DialogSystem/JDH_OnlyTestFolder/Question_mark.Question_mark'"));

	Question_mark = ObjectFinder.Object;
	Exclamation_mark = ObjectFinder2.Object;

	SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));
	timer = 0.0f;
	position = FVector(0.0f, 0.0f, 180.0f);
}

void AQuestIconActor::Init(FVector pos, AInteractionCharacter* OwnerChar)
{
	UE_LOG(LogTemp, Log, TEXT("AQuestIconActor 45 - Init is run"));
	position = FVector(0.0f, 0.0f, 180.0f);
	position += pos;
	OwnerInteractionChar = OwnerChar;
}

void AQuestIconActor::QuestIconUpdate(EQuestProgress quest_progress)
{
	UE_LOG(LogTemp, Log, TEXT("QuestIconActor 57 :: QuestIconUpdate execute name : %s, progress : %s"), 
		*AActor::GetDebugName(this), *GetEnumName(quest_progress));
	if (quest_progress == EQuestProgress::Ready)
	{
		StaticMeshComponent->SetStaticMesh(Question_mark);
		ProgressVisible = true;
	}
	else if (quest_progress == EQuestProgress::OnGoing)
	{
		ProgressVisible = false;
	}
	else if (quest_progress == EQuestProgress::Complete)
	{
		StaticMeshComponent->SetStaticMesh(Exclamation_mark);
		ProgressVisible = true;
	}
	else
	{
		ProgressVisible = false;
	}
	IconVisibleUpdate();
}

// Called when the game starts or when spawned
void AQuestIconActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("AQuestIconActor 80 - BeginPlay is run"));
}

// Called every frame
void AQuestIconActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!posinit)
	{
		SetActorLocation(position + OwnerInteractionChar->GetActorLocation());
		//SetUIVisible(true);
		posinit = true;
	}

	timer += DeltaTime;
	if (timer >= RotationCycle)
	{
		int i;
		for (i = 0; timer >= RotationCycle; i++)
			timer -= RotationCycle;

		FRotator rot_cam = this->GetActorRotation();
		rot_cam.Yaw += (QuestIconRotateSpeed * i);
		SetActorRotation(rot_cam);
	}
}

void AQuestIconActor::SetUIVisible(bool visible)
{
	VisibleInterCharOverride = visible;
	IconVisibleUpdate();
}

void AQuestIconActor::IconVisibleUpdate()
{
	this->SetActorHiddenInGame(!(VisibleInterCharOverride && ProgressVisible));
}