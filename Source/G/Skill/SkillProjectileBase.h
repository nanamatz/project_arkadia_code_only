#pragma once

#include "CoreMinimal.h"
#include "G/Skill/SkillBase.h"
#include "SkillProjectileBase.generated.h"

/**
 * 
 */

UCLASS()
class G_API USkillProjectileBase : public USkillBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TSubclassOf<class AProjectileBase> projectileClass;
public:

	USkillProjectileBase();
	
	virtual bool CheckSkillPossibility() override;

	virtual void SkillStart() override;

	virtual void UseSkill() override;

	virtual void SkillEnd() override;
	
	virtual void SpawnTargetProjectile(int _attackIndex = 0);
	
	virtual void ProjectileBeginOverlap(AActor* OtherActor, int attackIndex);
};
