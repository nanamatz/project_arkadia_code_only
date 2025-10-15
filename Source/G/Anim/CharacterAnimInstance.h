// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "G/Util/Define.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FSkillMontageDelegate);

UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	Idle,
	Burrow,
	Stun,
	Hit,
	Attack,
	Walk,
	Running,
	Jump,
	Death,
};

UCLASS()
class G_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EAnimationState AnimState;

	UPROPERTY(BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool bDeath = false;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* hit_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* death_montage;

	UPROPERTY(EditAnywhere, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* primary_attack_montage;
public:
	UCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetAnimState(EAnimationState state) { AnimState = state; }
	EAnimationState GetAnimState() const { return AnimState; }

	void PlayRespawnMontage();
	void PlayPrimaryAttackMontage();
	void PlayDeathMontage();
	void PlayHitMontage();

	void PlayBuffMontage(enum class EBUFFTYPE type);
	void PlayDebuffMontage(enum class EDEBUFFTYPE type);

	UFUNCTION(BlueprintCallable)
	void PlayAnimMontage(class UAnimMontage* _playMontage);

	TArray<FSkillMontageDelegate> PlayMontage;
};
