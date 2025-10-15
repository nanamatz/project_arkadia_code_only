// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "G/Skill/ProjectileBase.h"
#include "G/Skill/RuthCinda/TripleShot/SkillTripleShot.h"
#include "RuthCindaProjectile.generated.h"


/**
 * 
 */

UCLASS()
class G_API ARuthCindaProjectile : public AProjectileBase
{
	GENERATED_BODY()
	/*FTripleShotInfo _tripleShotInfo;

public:
	// Sets default values for this actor's properties
	ARuthCindaProjectile();

	virtual void SetSpawnProjectile
	(class USkillBase* skillClass, class AActor* spawnCharacter, int Type, float PDamage, float SRange, float Speed,
	 TWeakObjectPtr<USceneComponent> target) override;

	//		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult) override;*/
};
