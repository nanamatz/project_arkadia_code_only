// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "FernandoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class G_API UFernandoAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

	UFernandoAnimInstance();

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_darkpulse_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_to_hell_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_meteor_montage;

public:
	void PlayDarkPulseMontage();
	void PlayToHellMontage();
	void PlayMeteorMontage();

private:
	void AddDelegateFunction(void (UFernandoAnimInstance::*Function)());
};
