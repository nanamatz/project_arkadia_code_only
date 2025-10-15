// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "RuthCindaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class G_API URuthCindaAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

	URuthCindaAnimInstance();

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_triple_arrow_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_blazing_rain_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_volley_montage;

public :
	void PlayTripleArrowMontage();
	void PlayShadowStepMontage();
	void PlayAssassinationMontage();

private :
	void AddDelegateFunction(void (URuthCindaAnimInstance::*Function)());
};
