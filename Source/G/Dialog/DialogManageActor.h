// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogManageActor.generated.h"

UCLASS()
class G_API ADialogManageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogManageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UDialogComponentBase* DialogComponent;

	UPROPERTY()
	class UBehaviorTree* DialogTreeBase;

	UPROPERTY()
	class AInteractionCharacter* OwnerInterChar = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void init(class UBehaviorTree* DialogTreeBase, class AInteractionCharacter* oic);
	class UDialogComponentBase* GetDialogComponent();
	class AInteractionCharacter* GetOwnerChar() { return OwnerInterChar; }
};
