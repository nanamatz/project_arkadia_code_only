// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/ProjectileBase.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include <G/NonPlayerCharacter.h>
#include <Kismet/GameplayStatics.h>

#include "SkillBase.h"
#include "SkillInfoManager.h"
#include "SkillProjectileBase.h"
#include "Engine/EngineTypes.h"
#include "G/G.h"
#include "GameFramework/MovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SkillSystemComponent/SkillSystemComponent.h"

/*
    FernandoProjectile(AttackProjectile), DarkPulseProjectile이 유도탄 형태로 동작하게 함
    아직 제대로 동작하는지에 대한 확신은 없음
*/

// Sets default values
AProjectileBase::AProjectileBase()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("PlayerProjectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::BeginOverlap);
	//CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	RootComponent = CollisionComp;

	MovementEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileMovementEffect"));
	MovementEffect->SetupAttachment(RootComponent); //, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;

	/*
	    initspeed, maxspeed 부분 지움
	    유도탄 형태로 동작하지 않는 경우, initspeed, maxspeed를 초기화해주어야 함
	*/

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent); //, FAttachmentTransformRules::KeepRelativeTransform);

	InitialLifeSpan = 3.0f;
}

void AProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(isInit)
	{
		FVector projectilLocation = GetActorLocation();
		FVector characterLocation = myCharacter->GetActorLocation();
		float dis = FVector::Distance(characterLocation, projectilLocation);
		if(mySkill->GetSkillInfoManager()->GetCharacterRange() < dis)
		{
			ProjectileDestroy();
		}
	}
}

void AProjectileBase::SetCollisionIgnoreActor(AActor* ignoreActor)
{
	CollisionComp->MoveIgnoreActors.Add(ignoreActor);
}

void AProjectileBase::SetHomingTarget(TWeakObjectPtr<USceneComponent> target)
{
	ProjectileMovement->HomingTargetComponent = target;
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->HomingAccelerationMagnitude = 1000.0f;
}


void AProjectileBase::SetTargetSpawnProjectile(class USkillProjectileBase* _mySkill, class AGCharacter* spawnCharacter, int _attackIndex)
{
	isInit = false;
	//skillClass 각 프로젝타일에서 스킬 정보를 받기 위해 캐스팅하기
	myCharacter = spawnCharacter;
	mySkill = _mySkill;
	attackIndex = _attackIndex;
	SetCollisionIgnoreActor(spawnCharacter);
	if(!myCharacter && !mySkill)
	{
		LOG(Warning, "Not Find MyCharacter or MySkill");
		return;
	}

	auto target = myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	if(!target)
	{
		SetSpawnProjectile();
		return;
	}
	auto targetComponent = myCharacter->GetSkillSystemComponent()->GetSkillTarget()->GetRootComponent();
	SetHomingTarget(targetComponent);

	ProjectileMovement->InitialSpeed = 1500.0f;

	if (!IsValid(targetComponent))
	{
		FVector dirVec = GetActorForwardVector();
		dirVec = dirVec.GetSafeNormal();
		ProjectileMovement->Velocity = dirVec * ProjectileMovement->InitialSpeed;
		return;
	}
	FVector dirVec = (targetComponent->GetComponentLocation()) - spawnCharacter->GetActorLocation();
	dirVec = dirVec.GetSafeNormal();

	ProjectileMovement->Velocity = dirVec * ProjectileMovement->InitialSpeed;
	isInit = true;
}

void AProjectileBase::SetSpawnProjectile()
{
	ProjectileMovement->InitialSpeed = 1500.0f;

	FVector dirVec =  myCharacter->GetActorForwardVector();
	dirVec = dirVec.GetSafeNormal();

	ProjectileMovement->Velocity = dirVec * ProjectileMovement->InitialSpeed;
	isInit = true;
}

// 여기서는 이펙트, 삭제만 처리
void AProjectileBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	mySkill->ProjectileBeginOverlap(OtherActor, attackIndex);
	if (HitEffect)
	{
		UWorld* World = GetWorld();
		UGameplayStatics::SpawnEmitterAtLocation(World, HitEffect, GetActorTransform());
	}
	ProjectileDestroy();
}

void AProjectileBase::ProjectileDestroy()
{
	Destroy();
}
