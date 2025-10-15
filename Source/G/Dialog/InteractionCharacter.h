// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/NonPlayerCharacter.h"
#include "G/Dialog/FQuestManager.h"
#include "InteractionCharacter.generated.h"

UCLASS(BlueprintType)
class G_API AInteractionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInteractionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractionCollision;

	UPROPERTY()
	class UGGameInstance* GameInstance;

	UPROPERTY()
	class UDialogComponentBase* DialogComponent;

	int32 QuestIndex = -1;
	bool QuestIconVisible = false;

	UPROPERTY()
	class AQuestIconActor* QuestIconActor;

	UPROPERTY()
	class ADialogManageActor* DialogManageActor;

public:
	UPROPERTY(EditAnywhere)
	FVector QuestIconOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	FVector2D InfoOffset = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	FVector2D InteractIconOffset = FVector2D(0.0f, 0.0f);


	UPROPERTY(EditAnywhere)
	FString CharName;

	UPROPERTY(EditAnywhere)
	FString QuestHashTag;

	UPROPERTY(EditAnywhere)
	int32 NpcNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* DialogTree;

	UFUNCTION(BlueprintCallable)
	void RayShoot();

	void QuestInfoUpdate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActiveHighlight(FVector2D);
	void DeActiveHighlight();

	class UDialogComponentBase* GetDialogComponent();
	class ADialogManageActor* GetDialogManageActor() { return DialogManageActor; }

	void init();
	void SetQuestVisible(bool hidden);
	bool GetQuestVisible();
};
