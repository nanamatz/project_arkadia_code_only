// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DrakeAnimInstance.h"

UNPC_DrakeAnimInstance::UNPC_DrakeAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/Drake/Animation/AM_Drake_Attack.AM_Drake_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/Drake/Animation/AM_Drake_Death.AM_Drake_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/Drake/Animation/AM_Drake_HitReact.AM_Drake_HitReact'"));

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
