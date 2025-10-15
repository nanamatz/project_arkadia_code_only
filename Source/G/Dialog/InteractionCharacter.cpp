// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/InteractionCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include "G/Manager/InventoryManager.h"
#include "G/GGameInstance.h"
#include "G/Dialog/FQuestManager.h"
#include "G/Item/ItemBase.h"
#include "G/Dialog/QuestIconUI.h"
#include "G/Dialog/QuestIconActor.h"
#include "G/Dialog/DialogManageActor.h"
#include "G/UI/InteractionCharacterInfoUI.h"
#include "Components/WidgetComponent.h"
#include "G/Dialog/DialogComponentBase.h"
#include "G/Dialog/DialogWidgetBase.h"
#include "G/Dialog/InteractionInterface.h"
#include "G/UI/Map/PointOfInterestComponent.h"

// Sets default values
AInteractionCharacter::AInteractionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	QuestIconActor = nullptr;
	DialogManageActor = nullptr;
	GameInstance = nullptr;
	QuestIndex = -1;

	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(GetRootComponent());
	
	InteractionCollision->SetCollisionProfileName("InteractionCollision");
	InteractionCollision->SetCanEverAffectNavigation(false);
	InteractionCollision->SetSphereRadius(100.0f);
	InteractionCollision->bDynamicObstacle = true;
	//InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInteractionCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("InteractionChar 44 - PostInitializeComponents is execute Name - %s, DebugName - %s"),
		*(this->GetName()),
		*(AActor::GetDebugName(this)));

	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!QuestIconActor)
	{
		QuestIconActor = GetWorld()->SpawnActor<AQuestIconActor>();
		if (QuestIconActor)
			QuestIconActor->Init(QuestIconOffset,this);
	}

	if (!DialogManageActor)
	{
		DialogManageActor = GetWorld()->SpawnActor<ADialogManageActor>();
		if (DialogManageActor)
			DialogManageActor->init(DialogTree, this);
	}

	UE_LOG(LogTemp, Log, TEXT("InteractionChar 72 - QuestIndex %d"), QuestIndex);
	if (QuestIndex == -1)
	{
		if (GameInstance && GameInstance->QuestManager)
		{

			QuestIndex = GameInstance->QuestManager->GetHashTag(QuestHashTag);
		}
		else
			return;

		if (QuestIndex > -1)
		{
			GameInstance->QuestManager->GetQuestList()[QuestIndex]->AssignedCharacters[NpcNumber] = this;
		}
		QuestInfoUpdate();
		if (QuestIndex > -1)
		{
			SetActorHiddenInGame(!GameInstance->QuestManager->GetQuestList()[QuestIndex]->isVisibles[NpcNumber]);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("InteractionChar 86 - PostInitializeComponents is complete, Questicon Name - %s, DialogManager name - %s"),
		*(AActor::GetDebugName(QuestIconActor)), *(AActor::GetDebugName(DialogManageActor)));

}

void AInteractionCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Log, TEXT("InteractionChar 35 - BeginDestroy is execute"));
	if (QuestIconActor)
	{
		QuestIconActor->Destroy();
	}

	if (DialogManageActor)
	{
		DialogManageActor->Destroy();
	}
	if(GameInstance && GameInstance->QuestManager && (QuestIndex > -1))
		GameInstance->QuestManager->GetQuestList()[QuestIndex]->AssignedCharacters[NpcNumber] = nullptr;
}


// Called every frame
void AInteractionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("InteractionChar 109 - Tick is run"));
	//QuestIconActor->Init(GetActorLocation() + QuestIconOffset, rot);
	//RayShoot();
}

void AInteractionCharacter::RayShoot()
{
	FHitResult ray_result;

	FVector fff;

	fff = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	if (GetWorld()->LineTraceSingleByChannel(ray_result,
	                                         this->GetActorLocation(),
	                                         fff,
	                                         ECC_Visibility))
	{
		//UE_LOG(LogTemp, Log, TEXT("InteractionCharacter(84) ShootRay : fff.x fff.y fff.z : %f, %f, %f"),fff.X, fff.Y, fff.Z);
		//FVector ray =  ray_result.GetActor()->GetActorLocation();
		//UE_LOG(LogTemp, Log, TEXT("InteractionCharacter(85) ShootRay : ray.x ray.y ray.z : %f, %f, %f"), ray.X, ray.Y, ray.Z);
		QuestIconActor->SetUIVisible(false);
	}
	else
	{
		QuestIconActor->SetUIVisible(true);
	}
}

// Called to bind functionality to input
void AInteractionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//퀘스트 정보를 갱신하는 함수
//이걸 틱에다 넣어야 하나...?
void AInteractionCharacter::QuestInfoUpdate()
{
	if (QuestIndex < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate (89) - QuestIndex is -1"));
		return;
	}

	if (!GameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate (174) - GameInstance is null"));
		GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (!GameInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate (177) - GameInstance is null exeception"));
			return;
		}
	}



	if (GameInstance->QuestManager)
	{
		UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate 186 Questmanager is vaild"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate 190 Questmanager is not vaild"));
	}

	UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate 193 questarray num %d"),
		GameInstance->QuestManager->GetQuestList().Num());
	

	UQuestInfo* quest = GameInstance->QuestManager->GetQuestList()[QuestIndex];

	this->SetActorHiddenInGame(!(quest->isVisibles[NpcNumber]));

	if (QuestIconActor)
	{
		QuestIconActor->QuestIconUpdate(quest->QuestProgress);
		QuestIconActor->SetUIVisible(quest->isVisibles[NpcNumber]);
	}
	UE_LOG(LogTemp, Log, TEXT("InteractionCharacter::QuestInfoUpdate (95) - quest_progress is %s, quest index is %d"),
	       *GetEnumName<EQuestProgress>(quest->QuestProgress), QuestIndex);
}

void AInteractionCharacter::ActiveHighlight(FVector2D Position)
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AInteractionCharacter::DeActiveHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AInteractionCharacter::init()
{
	return;
}

UDialogComponentBase* AInteractionCharacter::GetDialogComponent()
{ 
	return DialogManageActor->GetDialogComponent(); 
}

void AInteractionCharacter::SetQuestVisible(bool hidden)
{
	if (QuestIndex > -1)
	{
		GameInstance->QuestManager->GetQuestList()[QuestIndex]->isVisibles[NpcNumber] = hidden;
		this->QuestInfoUpdate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AInteractionCharacter 204 SetQuestHidden :: QuestIndex is not vaild"));
	}
}

bool AInteractionCharacter::GetQuestVisible()
{
	if (QuestIndex > -1)
	{
		return GameInstance->QuestManager->GetQuestList()[QuestIndex]->isVisibles[NpcNumber];
	}
	//UE_LOG(LogTemp, Log, TEXT("AInteractionCharacter 213 GetQuestHidden :: QuestIndex is not vaild"));
	return false;
}