// Fill out your copyright notice in the Description page of Project Settings.


#include "SeeingThroughCollision.h"
#include "GCharacter.h"
#include "SeeingThroughActor.h"
#include "Components/BoxComponent.h"


// Sets default values
ASeeingThroughCollision::ASeeingThroughCollision()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SeeingThroughCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SeeingThroughCollision"));
	RootComponent = SeeingThroughCollision;
	SeeingThroughCollision->SetCollisionProfileName(TEXT("GPlayerCamera"));
}

// Called when the game starts or when spawned
void ASeeingThroughCollision::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASeeingThroughCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASeeingThroughCollision::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AGCharacter* Character = Cast<AGCharacter>(OtherActor);
	if (Character)
	{
		for (auto SeeingThroughActor : SeeingThroughActors)
		{
			SeeingThroughActor->SetShowSeeingThrough(true);
		}
	}
}

void ASeeingThroughCollision::NotifyActorEndOverlap(AActor* OtherActor)
{
	AGCharacter* Character = Cast<AGCharacter>(OtherActor);
	if (Character)
	{
		for (auto SeeingThroughActor : SeeingThroughActors)
		{
			SeeingThroughActor->SetShowSeeingThrough(false);
		}
	}
}
