// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeeingThroughCollision.generated.h"

UCLASS()
class G_API ASeeingThroughCollision : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASeeingThroughCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class UBoxComponent* SeeingThroughCollision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<class ASeeingThroughActor*> SeeingThroughActors;
};
