// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_GuardianAnimInstance.h"

UNPC_GuardianAnimInstance::UNPC_GuardianAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/Guardian/Animation/AM_Guardian_Attack.AM_Guardian_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/Guardian/Animation/AM_Guardian_Death.AM_Guardian_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/Guardian/Animation/AM_Guardian_HitReact.AM_Guardian_HitReact'"));

	if (primary_attack_object.Succeeded())
	{
		primary_attack_montage = primary_attack_object.Object;
	}

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}
}

