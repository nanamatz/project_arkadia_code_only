// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UENUM()
// COMMON 일반, PIERCE 관통
enum PROJECTILETYPE { COMMON, PIERCE };

USTRUCT()
struct FProjectileInfo
{
	GENERATED_USTRUCT_BODY()
	PROJECTILETYPE Type;
	// 데미지
	float Damage;
	// 사거리
	float Range;
	// 속도
	float Speed;
};

UCLASS()
class G_API AProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	bool isInit = false;
	
	UPROPERTY()
	class AGCharacter* myCharacter;

	UPROPERTY()
	class USkillProjectileBase* mySkill;

	// 투사체 정보
	UPROPERTY()
	struct FProjectileInfo ProjectileInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UParticleSystemComponent* MovementEffect;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitEffect;

	UPROPERTY()
	int attackIndex;

	// 발사체 제거
	virtual void ProjectileDestroy();

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	void SetCollisionIgnoreActor(class AActor* ignoreActor);

	void SetHomingTarget(TWeakObjectPtr<USceneComponent> target);

	virtual void SetTargetSpawnProjectile
	(class USkillProjectileBase* _mySkill, class AGCharacter* spawnCharacter, int _attackIndex = 0);

	virtual void SetSpawnProjectile();

	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);
};
