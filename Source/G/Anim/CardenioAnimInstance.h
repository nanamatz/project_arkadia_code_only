// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "CardenioAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class G_API UCardenioAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

	UCardenioAnimInstance();

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_strike_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_barrier_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_justice_montage;

public:
	UFUNCTION()
	void PlayStrikeMontage();
	UFUNCTION()
	void PlayBarrierMontage();
	UFUNCTION()
	void PlayJusticeMontage();

private:
	void AddDelegateFunction(void (UCardenioAnimInstance::*Function)());
};
