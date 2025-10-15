// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Skill/CardenioSkill/Advent/SkillAdvent.h"
#include <G/PlayerCharacter.h>

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

bool USkillAdvent::CheckSkillPossibility()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(_myCharacter->GetWorld());
	FVector playerPos = _myCharacter->GetActorLocation();
	FVector skillPos = _myCharacter->GetSkillSystemComponent()->GetSkillPos();

	UNavigationPath* NavPath = NavSystem->
		FindPathToLocationSynchronously(_myCharacter->GetWorld(), playerPos, skillPos);

	if (NavPath && NavPath->IsValid() && NavPath->PathPoints.Num() > 0 && Super::CheckSkillPossibility())
	{
		return true;
	}
	return false;
}

void USkillAdvent::SkillStart()
{
	totalDamage = 0;
	// 애니메이션 실행
	Super::SkillStart();
	FTimerHandle moveTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(moveTimeHandle, [this]()
	                                       {
		                                       FVector targetPos = _myCharacter->GetSkillSystemComponent()->
			                                       GetSkillPos();
		                                       targetPos.Z += 50.0f;
		                                       _myCharacter->SetActorLocation(targetPos);
	                                       },
	                                       GetSkillInfoManager()->GetSkillCastTime() - 0.3f, false);
}

void USkillAdvent::UseSkill()
{
	Super::UseSkill();
	// 떨어진 후 데미지 처리
	SkillAttack();
}

void USkillAdvent::SkillEnd()
{
}

void USkillAdvent::SkillAttack()
{
	totalDamage = GetOverlapObjectAttack();
	for(auto& action : GetSkillInfoManager()->GetActionList())
	{
		action->SkillAction(_myCharacter, this);
	}
}
