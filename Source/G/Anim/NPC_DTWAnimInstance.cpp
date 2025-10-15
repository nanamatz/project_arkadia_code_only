// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/NPC_DTWAnimInstance.h"

UNPC_DTWAnimInstance::UNPC_DTWAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/thieves/DTW_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/thieves/DTW_Buff'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/thieves/DTW_React'"));

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
