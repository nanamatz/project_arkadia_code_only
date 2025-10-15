// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/NPC_BuffBlueAnimInstance.h"

UNPC_BuffBlueAnimInstance::UNPC_BuffBlueAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buf_Blue/Fire_Montage.Fire_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buf_Blue/Hitreat_Fwd_Montage.Hitreat_Fwd_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buf_Blue/Death_Bwd_Montage.Death_Bwd_Montage'"));

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
