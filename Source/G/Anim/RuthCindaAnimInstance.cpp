// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/RuthCindaAnimInstance.h"

URuthCindaAnimInstance::URuthCindaAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/Character/RuthCinda/Attack/AM_Attack.AM_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_triple_arrow_object(
		TEXT("AnimMontage'/Game/Character/RuthCinda/Skill_TripleArrow/AM_TripleArrow.AM_TripleArrow'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_blazing_rain_object(
		TEXT("AnimMontage'/Game/Character/RuthCinda/Skill_BlazingRain/AM_BlazingRain.AM_BlazingRain'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_volley_object(
		TEXT("AnimMontage'/Game/Character/RuthCinda/Skill_Volley/AM_Volley.AM_Volley'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Cardenio_Death.Cardenio_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Cardenio_Hit.Cardenio_Hit'"));

	if (primary_attack_object.Succeeded())
	{
		primary_attack_montage = primary_attack_object.Object;
	}

	if (skill_triple_arrow_object.Succeeded())
	{
		skill_triple_arrow_montage = skill_triple_arrow_object.Object;
	}

	if (skill_blazing_rain_object.Succeeded())
	{
		skill_blazing_rain_montage = skill_blazing_rain_object.Object;
	}

	if (skill_volley_object.Succeeded())
	{
		skill_volley_montage = skill_volley_object.Object;
	}

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}

	AddDelegateFunction(&URuthCindaAnimInstance::PlayPrimaryAttackMontage);
	AddDelegateFunction(&URuthCindaAnimInstance::PlayTripleArrowMontage);
	AddDelegateFunction(&URuthCindaAnimInstance::PlayShadowStepMontage);
	AddDelegateFunction(&URuthCindaAnimInstance::PlayAssassinationMontage);
}

void URuthCindaAnimInstance::PlayTripleArrowMontage()
{
	if (!Montage_IsPlaying(skill_triple_arrow_montage))
	{
		Montage_Play(skill_triple_arrow_montage);
	}
}

void URuthCindaAnimInstance::PlayShadowStepMontage()
{
	if (!Montage_IsPlaying(skill_blazing_rain_montage))
	{
		Montage_Play(skill_blazing_rain_montage);
	}
}

void URuthCindaAnimInstance::PlayAssassinationMontage()
{
	if (!Montage_IsPlaying(skill_volley_montage))
	{
		Montage_Play(skill_volley_montage);
	}
}

void URuthCindaAnimInstance::AddDelegateFunction(void (URuthCindaAnimInstance::*Function)())
{
	FSkillMontageDelegate NewDelegate;
	NewDelegate.BindUObject(this, Function);
	PlayMontage.Add(NewDelegate);
}
