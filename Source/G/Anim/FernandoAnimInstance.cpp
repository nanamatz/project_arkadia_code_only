// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/FernandoAnimInstance.h"

UFernandoAnimInstance::UFernandoAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/Character/Fernando/Attack/AM_Attack.AM_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_darkpulse_object(
		TEXT("AnimMontage'/Game/Character/Fernando/Skill_Dark_Pulse/AM_DarkPulse.AM_DarkPulse'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_to_hell_object(
		TEXT("AnimMontage'/Game/Character/Fernando/Skill_To_Hell/AM_ToHell.AM_ToHell'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_meteor_object(
		TEXT("AnimMontage'/Game/Character/Fernando/Skill_Meteor/AM_Meteor.AM_Meteor'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/Character/Fernando/Fernando_Hitreact.Fernando_Hitreact'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/Character/Fernando/Fernando_Death.Fernando_Death'"));

	if (primary_attack_object.Succeeded())
	{
		primary_attack_montage = primary_attack_object.Object;
	}

	if (skill_darkpulse_object.Succeeded())
	{
		skill_darkpulse_montage = skill_darkpulse_object.Object;
	}

	if (skill_to_hell_object.Succeeded())
	{
		skill_to_hell_montage = skill_to_hell_object.Object;
	}

	if (skill_meteor_object.Succeeded())
	{
		skill_meteor_montage = skill_meteor_object.Object;
	}

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}

	AddDelegateFunction(&UFernandoAnimInstance::PlayPrimaryAttackMontage);
	AddDelegateFunction(&UFernandoAnimInstance::PlayDarkPulseMontage);
	AddDelegateFunction(&UFernandoAnimInstance::PlayToHellMontage);
	AddDelegateFunction(&UFernandoAnimInstance::PlayMeteorMontage);
}

void UFernandoAnimInstance::PlayDarkPulseMontage()
{
	if (!Montage_IsPlaying(skill_darkpulse_montage))
	{
		Montage_Play(skill_darkpulse_montage);
	}
}

void UFernandoAnimInstance::PlayToHellMontage()
{
	if (!Montage_IsPlaying(skill_to_hell_montage))
	{
		Montage_Play(skill_to_hell_montage);
	}
}

void UFernandoAnimInstance::PlayMeteorMontage()
{
	if (!Montage_IsPlaying(skill_meteor_montage))
	{
		Montage_Play(skill_meteor_montage);
	}
}

void UFernandoAnimInstance::AddDelegateFunction(void (UFernandoAnimInstance::*Function)())
{
	FSkillMontageDelegate NewDelegate;
	NewDelegate.BindUObject(this, Function);
	PlayMontage.Add(NewDelegate);
}
