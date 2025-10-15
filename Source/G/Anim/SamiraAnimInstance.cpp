// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/SamiraAnimInstance.h"

USamiraAnimInstance::USamiraAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/Character/Samira/Attack/AM_Attack.AM_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_cut_throat_object(
		TEXT("AnimMontage'/Game/Character/Samira/Skill_CutThroat/AM_CutThroat.AM_CutThroat'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_shadow_step_object(
		TEXT("AnimMontage'/Game/Character/Samira/Skill_ShadowStep/AM_ShadowStep.AM_ShadowStep'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_Assassination_object(
		TEXT("AnimMontage'/Game/Character/Samira/Skill_Assassination/AM_assassination.AM_assassination'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Cardenio_Death.Cardenio_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Cardenio_Hit.Cardenio_Hit'"));

	if (primary_attack_object.Succeeded())
	{
		primary_attack_montage = primary_attack_object.Object;
	}

	if (skill_cut_throat_object.Succeeded())
	{
		skill_cut_throat_montage = skill_cut_throat_object.Object;
	}

	if (skill_shadow_step_object.Succeeded())
	{
		skill_shadow_step_montage = skill_shadow_step_object.Object;
	}

	if (skill_Assassination_object.Succeeded())
	{
		skill_Assassination_montage = skill_Assassination_object.Object;
	}

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}

	AddDelegateFunction(&USamiraAnimInstance::PlayPrimaryAttackMontage);
	AddDelegateFunction(&USamiraAnimInstance::PlayCutThroatMontage);
	AddDelegateFunction(&USamiraAnimInstance::PlayShadowStepMontage);
	AddDelegateFunction(&USamiraAnimInstance::PlayAssassinationMontage);
}

void USamiraAnimInstance::PlayCutThroatMontage()
{
	if (!Montage_IsPlaying(skill_cut_throat_montage))
	{
		Montage_Play(skill_cut_throat_montage);
	}
}

void USamiraAnimInstance::PlayShadowStepMontage()
{
	if (!Montage_IsPlaying(skill_shadow_step_montage))
	{
		Montage_Play(skill_shadow_step_montage);
	}
}

void USamiraAnimInstance::PlayAssassinationMontage()
{
	if (!Montage_IsPlaying(skill_Assassination_montage))
	{
		Montage_Play(skill_Assassination_montage);
	}
}

void USamiraAnimInstance::AddDelegateFunction(void (USamiraAnimInstance::*Function)())
{
	FSkillMontageDelegate NewDelegate;
	NewDelegate.BindUObject(this, Function);
	PlayMontage.Add(NewDelegate);
}
