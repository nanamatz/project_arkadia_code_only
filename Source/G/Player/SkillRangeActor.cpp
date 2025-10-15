// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Player/SkillRangeActor.h"

#include "Components/SphereComponent.h"
#include "G/GCharacter.h"
#include "G/PlayerCharacter.h"

// Sets default values
ASkillRangeActor::ASkillRangeActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(Mesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	Mesh->SetStaticMesh(SphereMesh.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMaterial(
		TEXT("Material'/Game/CombatSystem/Materials/M_SkillRange.M_SkillRange'"));
	Mesh->SetMaterial(0, SphereMaterial.Object);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkillCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SkillCollision"));
	SkillCollision->SetupAttachment(Mesh);
	SkillCollision->SetCollisionProfileName(TEXT("SkillCollision"));
	SkillCollision->SetRelativeLocation(FVector::ZeroVector);

	SkillCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkillRangeActor::OnSkillCollisionBeginOverlap);
	SkillCollision->OnComponentEndOverlap.AddDynamic(this, &ASkillRangeActor::OnSkillCollisionEndOverlap);


	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASkillRangeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASkillRangeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkillRangeActor::ActiveSkillRange(APlayerCharacter* myCharacter, float radius, ESkillTargetType targetType)
{
	float meshScale = radius / 100.0f * 2.0f;
	_targetType = targetType;
	switch (_targetType)
	{
	case ESkillTargetType::ALL:
		SkillCollision->SetCollisionProfileName(TEXT("SkillCollision"));
		break;
	case ESkillTargetType::ALLIgnoreMe:
		SkillCollision->IgnoreActorWhenMoving(myCharacter, true);
		SkillCollision->SetCollisionProfileName(TEXT("SkillCollision"));
		break;
	case ESkillTargetType::PLAYER:
		SkillCollision->SetCollisionProfileName(TEXT("PlayerTarget"));
		break;
	case ESkillTargetType::NPC:
		SkillCollision->SetCollisionProfileName(TEXT("NPCTarget"));
		break;
	default:
		SkillCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}

	Mesh->SetRelativeScale3D(FVector(meshScale, meshScale, meshScale));
	SkillCollision->SetSphereRadius(radius / meshScale);
	SetActorHiddenInGame(false);
}

void ASkillRangeActor::DeActiveSkillRange()
{
	SkillCollision->SetCollisionProfileName(TEXT("NoCollision"));
	for (AGCharacter* character : _overlapCharacters)
	{
		if(character)
			character->DeActiveHighlight();
	}
	_overlapCharacters.Empty();
	SetActorLocation(FVector(0, 0, 5000.0f));
	SetActorHiddenInGame(true);
}

void ASkillRangeActor::OnSkillCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	AGCharacter* character = Cast<AGCharacter>(OtherActor);
	if (character)
	{
		if (!character->GetBDeath())
		{
			character->ActiveHighlight();
			_overlapCharacters.Add(character);
		}
	}
}

void ASkillRangeActor::OnSkillCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGCharacter* character = Cast<AGCharacter>(OtherActor);

	if (character)
	{
		Cast<AGCharacter>(OtherActor)->DeActiveHighlight();
	}
}
