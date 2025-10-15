#include "JumpAction.h"

#include "G/G.h"
#include "G/PlayerCharacter.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UJumpAction::SkillAction(AGCharacter* myCharacter, USkillBase* skill)
{
	Super::SkillAction(myCharacter, skill);

	auto characterAnimInstance = Cast<UCharacterAnimInstance>(myCharacter->GetMesh()->GetAnimInstance());
	characterAnimInstance->SetAnimState(EAnimationState::Jump);
	characterAnimInstance->StopAllMontages(0.1f);

	JumpAction();
}

void UJumpAction::JumpAction()
{
	UWorld* World = _myCharacter->GetWorld();

	//_myCharacter->Jump();

	FVector val;
	FVector startPos = _myCharacter->GetActorLocation();
	FVector endPos = _myCharacter->GetSkillSystemComponent()->GetSkillTarget()->GetActorLocation();

	// 200.0f <- Mesh 사이즈나 Capsule Component 사이즈로 바꿔도 될 듯
	endPos = startPos + (endPos - startPos).GetSafeNormal() * (FVector::Distance(startPos, endPos) - 200.0f);
	endPos.Z += 100.0f;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(World, val, _myCharacter->GetActorLocation(),
	                                                      endPos);

	_myCharacter->LaunchCharacter(val, true, true);

	auto characterAnimInstance = Cast<UCharacterAnimInstance>(_myCharacter->GetMesh()->GetAnimInstance());
}
