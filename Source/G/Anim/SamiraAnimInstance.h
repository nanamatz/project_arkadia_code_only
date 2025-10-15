// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "SamiraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class G_API USamiraAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

	USamiraAnimInstance();

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_cut_throat_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_shadow_step_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_Assassination_montage;

public:
	void PlayCutThroatMontage();
	void PlayShadowStepMontage();
	void PlayAssassinationMontage();

private:
	void AddDelegateFunction(void (USamiraAnimInstance::*Function)());
};
