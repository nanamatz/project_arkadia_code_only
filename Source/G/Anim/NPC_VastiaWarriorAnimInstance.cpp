#include "NPC_VastiaWarriorAnimInstance.h"

UNPC_VastiaWarriorAnimInstance::UNPC_VastiaWarriorAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> primary_attack_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Vastia/Warrior/Attack_Montage.Attack_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> death_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Vastia/Warrior/Hit_Montage.Hit_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_object(
		TEXT("AnimMontage'/Game/CombatSystem/NPC/Vastia/Warrior/Hit_Montage.Hit_Montage'"));

	if (primary_attack_object.Succeeded())
	{
		primary_attack_montage = primary_attack_object.Object;
	}

	if (death_object.Succeeded())
	{
		death_montage = death_object.Object;
	}

	if (hit_object.Succeeded())
	{
		hit_montage = hit_object.Object;
	}
}
