// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/NPC_BuffGreenAnimInstance.h"

UNPC_BuffGreenAnimInstance::UNPC_BuffGreenAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buff_Green/Knock_Fwd_Montage.Knock_Fwd_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buff_Green/Knock_Bwd_Montage.Knock_Bwd_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_boom_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Minion/Buff_Green/Knock_Fwd_Montage.Knock_Fwd_Montage'"));

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}

	if (skill_boom_object.Succeeded())
	{
		skill_boom_montage = skill_boom_object.Object;
	}
}

void UNPC_BuffGreenAnimInstance::PlaySkillBoomMontage()
{
	if (!Montage_IsPlaying(skill_boom_montage))
	{
		Montage_Play(skill_boom_montage);
	}
}
