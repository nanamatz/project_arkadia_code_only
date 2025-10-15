#include "SkillProjectileBase.h"

#include "ProjectileBase.h"
#include "SkillInfoManager.h"
#include "G/G.h"
#include "G/GCharacter.h"
#include "Particles/ParticleSystemComponent.h"

USkillProjectileBase::USkillProjectileBase()
{
}

bool USkillProjectileBase::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillProjectileBase::SkillStart()
{
	Super::SkillStart();
}

void USkillProjectileBase::UseSkill()
{
	Super::UseSkill();
	SpawnTargetProjectile();
}

void USkillProjectileBase::SkillEnd()
{
	Super::SkillEnd();
}

void USkillProjectileBase::SpawnTargetProjectile(int _attackIndex)
{
	if (projectileClass)
	{
		UWorld* World = _myCharacter->GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = _myCharacter;
			FVector SpawnLocation = _myCharacter->GetMesh()->GetSocketLocation(FName("ProjectileSocket"));
			FRotator Rotation = _myCharacter->GetActorForwardVector().Rotation();
			FTransform projectileTransform;
			projectileTransform.SetLocation(SpawnLocation);
			projectileTransform.SetRotation(FQuat(Rotation));
			AProjectileBase* Projectile = World->SpawnActorDeferred<AProjectileBase>(
				projectileClass, projectileTransform, _myCharacter);

			if (Projectile)
			{
				// 현재 스킬의 데미지를 발사체에 적용, 폭발 범위 전달
				Projectile->SetTargetSpawnProjectile(this, _myCharacter, _attackIndex);
			}
			Projectile->FinishSpawning(projectileTransform);
		}
	}
	else
	{
		LOG(Warning, "Not Find ProjectilClass");
	}
}

void USkillProjectileBase::ProjectileBeginOverlap(AActor* OtherActor, int attackIndex = 0)
{
	// Projectil 충돌 시 처리 기입
	if(OtherActor->IsA<AGCharacter>())
	{
		AGCharacter* target = Cast<AGCharacter>(OtherActor);
		FSkillCastAttack* attack = nullptr;
		if(GetSkillInfoManager()->GetAttackList().IsValidIndex(attackIndex))
		{
			attack = new FSkillCastAttack(GetSkillInfoManager()->GetAttackList()[attackIndex]);
		}
		TargetAttack(target, attack);
		TargetBuff(target);
		TargetDebuff(target);
	}
}
