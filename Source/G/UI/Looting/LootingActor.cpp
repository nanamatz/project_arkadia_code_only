// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingActor.h"
#include <Kismet/GameplayStatics.h>
#include "G/GGameInstance.h"
#include "G/Item/ItemBase.h"
#include "G/Manager/InventoryManager.h"
#include "Components/SphereComponent.h"
#include "LootingComponent.h"
#include "G/UI/InteractionCharacterInfoActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALootingActor::ALootingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the class finder in the constructor

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;

	// LootingRange Setting
	LootingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LootingCollision"));
	LootingCollision->SetupAttachment(GetRootComponent());
	
	LootingCollision->SetCollisionProfileName("InteractionCollision");
	LootingCollision->SetCanEverAffectNavigation(false);
	LootingCollision->SetSphereRadius(100.0f);
	LootingCollision->bDynamicObstacle = true;
	
	LootingCollision->OnComponentBeginOverlap.
	                    AddDynamic(this, &ALootingActor::OnLootingCollisionBeginOverlap);

	lootingcomponent = CreateDefaultSubobject<ULootingComponent>(TEXT("LootingComponent"));

	GameInstance = nullptr;
	//InteractionCharacterInfoActor = nullptr;
}

// Called when the game starts or when spawned
void ALootingActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALootingActor::ActiveHighlight(FVector2D Position)
{
	StaticMeshComponent->SetRenderCustomDepth(true);

	//GetMesh()->SetRenderCustomDepth(true);

	FVector2D InfoPosition = FVector2D(InfoOffset);
	//InteractionCharacterInfoActor->Active();
}

void ALootingActor::DeActiveHighlight()
{
	StaticMeshComponent->SetRenderCustomDepth(false);
	//InteractionCharacterInfoActor->DeActive();
}

void ALootingActor::OpenLootingUI(FVector2D pos)
{
	lootingcomponent->OpenLootingUI(pos);
}

void ALootingActor::CloseLootingUI()
{
	lootingcomponent->CloseLootingUI();
}

bool ALootingActor::isLootingEmpty()
{
	return lootingcomponent->isEmpty();
}

void ALootingActor::OnLootingCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                     bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("ALootingActor OnLootingCollisinonBeginOverlap"));
}


