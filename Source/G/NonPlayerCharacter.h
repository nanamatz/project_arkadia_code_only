// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatArea.h"
#include "GCharacter.h"
#include "NonPlayerCharacter.generated.h"

UCLASS()
class G_API ANonPlayerCharacter : public AGCharacter
{
	GENERATED_BODY()

protected:
	// 전투지역
	UPROPERTY()
	class ACombatArea* CombatArea;

	UPROPERTY()
	bool BControlling;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectingCollision;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* WarningCollision;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UPointOfInterestComponent* POIComoponent;
	
	UPROPERTY(EditAnywhere, Category = "CombatSet")
	float DetectingRange = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "CombatSet")
	float WarningRange = 2000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Reward")
	int32 RewardGold;

	FTimerHandle MyDelayTimerHandle;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class ULootingComponent* Lootingcomponent;
	
public:
	// Sets default values for this character's properties
	ANonPlayerCharacter();

	// 스폰되는 NPC 정보 세팅
	UFUNCTION(BlueprintCallable)
	void SetSpawnNPC(FString NPCName);

	FORCEINLINE bool GetBControlling() { return BControlling; }
	void SetBControlling(bool b);

	void SetCombatArea(ACombatArea* CA);
	void SetSpawnLocation(const FVector& location);

	UFUNCTION()
	ACombatArea* GetCombatArea();

	void StartCombat();

	// 데미지 처리
	virtual int32 TakeAttackDamage(int32 Damage, bool bPiercingDamage = false) override;
	virtual void Death() override;

	class APlayerCharacter* FindClosestTarget();
	bool CheckAttackPossibility(AGCharacter* Target, float Radius);
	bool TargetAttack(AGCharacter* Target);

	// 보상설정
	UFUNCTION()
	void SetRewardItem(TArray<struct FRewardItemSetting> RewardItemSettingList);
	UFUNCTION()
	void AddRewardItem(TSubclassOf<class UItemBase> RewardItem);

	UFUNCTION()
	void SetRewardGold(int Gold) { RewardGold = Gold; }

	UFUNCTION(BlueprintCallable)
	void OnDetectingCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnWarningCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                    const FHitResult& SweepResult);

	// 루팅 UI
	void OpenLootingUI(FVector2D pos);
	void CloseLootingUI();
	bool isLootingEmpty();
};
