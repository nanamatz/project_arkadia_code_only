// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/RuthCindaProjectile/RuthCindaProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"


/*ARuthCindaProjectile::ARuthCindaProjectile()
{
	ProjectileMovement->bIsHomingProjectile = true;
}

void ARuthCindaProjectile::SetSpawnProjectile(USkillBase* skillClass, AActor* spawnCharacter, int Type, float PDamage,
                                              float SRange, float Speed, TWeakObjectPtr<USceneComponent> target)
{
	_tripleShotInfo = Cast<USkillTripleShot>(skillClass)->GetTripleShotInfo();

	Super::SetSpawnProjectile(skillClass, spawnCharacter, Type, PDamage, SRange, Speed, target);
}

//void ARuthCindaProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//    Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
//}

void ARuthCindaProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (ProjectileInfo.Type == COMMON)
	{
		// 충돌한 오브젝트가 적 캐릭터인 경우 데미지 처리
		if (CheckNPC(OtherActor))
		{
			ANonPlayerCharacter* Target = Cast<ANonPlayerCharacter>(OtherActor);

			if (_tripleShotInfo.bMagic)
			{
			}
			if (_tripleShotInfo.bSteal)
			{
			}
			if (_tripleShotInfo.bCriticalUp)
			{
			}
			if (_tripleShotInfo.bOneShot)
			{
			}

			Target->TakeAttackDamage(ProjectileInfo.Damage);
			ProjectileDestroy();
		}
	}
}*/
