// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_KillCrabAnimInstance.h"

UNPC_KillCrabAnimInstance::UNPC_KillCrabAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/KillCrab/Animation/AM_KillCrab_Attack.AM_KillCrab_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/KillCrab/Animation/AM_KillCrab_Death.AM_KillCrab_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/KillCrab/Animation/AM_KillCrab_HitReact.AM_KillCrab_HitReact'"));

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


