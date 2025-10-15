// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootingActor.generated.h"

UCLASS()
class G_API ALootingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALootingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UGGameInstance* GameInstance;

	bool isGive = false;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class ULootingComponent* lootingcomponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* LootingCollision;

	//UPROPERTY()
	//class AInteractionCharacterInfoActor* InteractionCharacterInfoActor;

public:
	UPROPERTY(EditAnywhere)
	FString CharName;

	UPROPERTY(EditAnywhere)
	FVector2D InfoOffset = FVector2D(0.0f, 0.0f);

	UFUNCTION(BlueprintCallable)
	void OnLootingCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Highlighting Actor
	void ActiveHighlight(FVector2D Position);
	void DeActiveHighlight();

	//Control LootingComponent
	void OpenLootingUI(FVector2D pos);
	void CloseLootingUI();
	bool isLootingEmpty();
};
