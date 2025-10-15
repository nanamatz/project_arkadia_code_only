// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/NPC_BuffRedAnimInstance.h"

UNPC_BuffRedAnimInstance::UNPC_BuffRedAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buff_Red/Attack_BigSmash_Montage.Attack_BigSmash_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buff_Red/Death_Back_Montage.Death_Back_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buff_Red/Knock_Fwd_Montage.Knock_Fwd_Montage'"));

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
