// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_DustVenomAnimInstance.h"

UNPC_DustVenomAnimInstance::UNPC_DustVenomAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/DustVenom/Animation/AM_DustVenom_Attack.AM_DustVenom_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/DustVenom/Animation/AM_DustVenom_Death.AM_DustVenom_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Neutral/Monster/DustVenom/Animation/AM_DustVenom_HitReact.AM_DustVenom_HitReact'"));

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
