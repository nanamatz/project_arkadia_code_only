// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "NPC_BuffGreenAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class G_API UNPC_BuffGreenAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

	UNPC_BuffGreenAnimInstance();

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* skill_boom_montage;

public:
	void PlaySkillBoomMontage();
};
