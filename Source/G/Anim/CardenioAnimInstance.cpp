// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Anim/CardenioAnimInstance.h"
#include "CardenioAnimInstance.h"

UCardenioAnimInstance::UCardenioAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Attack/AM_Attack.AM_Attack'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_strike_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/SKill_Strike/AM_Strike.AM_Strike'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_barrier_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Skill_Barrier/AM_Barrier.AM_Barrier'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill_justice_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Skill_Justice/AM_Justice.AM_Justice'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Cardenio_Death.Cardenio_Death'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/Character/Cardenio/Cardenio_Hit.Cardenio_Hit'"));

	if (primary_attack_object.Succeeded())
	{
		primary_attack_montage = primary_attack_object.Object;
	}

	if (skill_strike_object.Succeeded())
	{
		skill_strike_montage = skill_strike_object.Object;
	}

	if (skill_barrier_object.Succeeded())
	{
		skill_barrier_montage = skill_barrier_object.Object;
	}

	if (skill_justice_object.Succeeded())
	{
		skill_justice_montage = skill_justice_object.Object;
	}

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}

	AddDelegateFunction(&UCardenioAnimInstance::PlayPrimaryAttackMontage);
	AddDelegateFunction(&UCardenioAnimInstance::PlayStrikeMontage);
	AddDelegateFunction(&UCardenioAnimInstance::PlayBarrierMontage);
	AddDelegateFunction(&UCardenioAnimInstance::PlayJusticeMontage);
}

void UCardenioAnimInstance::PlayStrikeMontage()
{
	if (!Montage_IsPlaying(skill_strike_montage))
	{
		Montage_Play(skill_strike_montage);
	}
}

void UCardenioAnimInstance::PlayBarrierMontage()
{
	if (!Montage_IsPlaying(skill_barrier_montage))
	{
		Montage_Play(skill_barrier_montage);
	}
}

void UCardenioAnimInstance::PlayJusticeMontage()
{
	if (!Montage_IsPlaying(skill_justice_montage))
	{
		Montage_Play(skill_justice_montage);
	}
}

void UCardenioAnimInstance::AddDelegateFunction(void (UCardenioAnimInstance::*Function)())
{
	FSkillMontageDelegate NewDelegate;
	NewDelegate.BindUObject(this, Function);
	PlayMontage.Add(NewDelegate);
}
