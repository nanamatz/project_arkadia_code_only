// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/GGameInstance.h"
#include "GameFramework/Actor.h"
#include "SkillData/SkillType/SkillCastAttack.h"
#include "SkillSpawnObjectBase.generated.h"

UCLASS()
class G_API ASkillSpawnObjectBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* areaCollision;

	UPROPERTY()
	class AGCharacter* myCharacter;

	UPROPERTY()
	class USkillBase* mySkill;

	UPROPERTY()
	bool bInit = false;
	int durationTurn;
	float attackDamage;
	float areaSize;
	bool bCountTurn;
	
	struct FSkillCastAttack attakcInfo;
	
	UPROPERTY()
	class UGGameInstance* GameInstance;
	
	UPROPERTY()
	TMap<class AGCharacter*, bool> attackAreaCharacterList;


public:
	ASkillSpawnObjectBase();

	virtual void Init(class USkillBase* mySkill, class AGCharacter* myCharacter);
	virtual void Tick(float DeltaTime) override;

	void TurnCount();

	virtual void MoveCharacterAttack();


	// overlap
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
