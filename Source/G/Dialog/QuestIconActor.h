// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "G/Dialog/FQuestManager.h"
#include "QuestIconActor.generated.h"

UCLASS()
class G_API AQuestIconActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestIconActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* QuestIconWidgetComponent;

	UPROPERTY()
	class UStaticMesh* Question_mark;

	UPROPERTY()
	class UStaticMesh* Exclamation_mark;

	float QuestIconRotateSpeed = 20.0f;

	FVector position = FVector(0.0f, 0.0f, 180.0f);

	UPROPERTY()
	class AInteractionCharacter* OwnerInteractionChar;

	bool posinit = false;

	float timer = 0.0f;

	float RotationCycle = 0.1f;

	bool VisibleInterCharOverride = true;

	bool ProgressVisible = true;

	void IconVisibleUpdate();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(FVector pos, class AInteractionCharacter* OwnerChar);
	void QuestIconUpdate(EQuestProgress state);
	void SetUIVisible(bool visible);
};
