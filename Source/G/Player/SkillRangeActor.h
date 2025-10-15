// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "G/Util/Define.h"
#include "SkillRangeActor.generated.h"

UCLASS()
class G_API ASkillRangeActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SkillCollision;

	UPROPERTY()
	TArray<class AGCharacter*> _overlapCharacters;

	ESkillTargetType _targetType;

public:
	// Sets default values for this actor's properties
	ASkillRangeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActiveSkillRange(class APlayerCharacter* myCharacter, float radius, ESkillTargetType targetType = ESkillTargetType::NONE);
	void DeActiveSkillRange();

	UFUNCTION()
	void OnSkillCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);

	UFUNCTION()
	void OnSkillCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
