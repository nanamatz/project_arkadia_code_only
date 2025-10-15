// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSpawnCharacter.h"

#include "G/GCharacter.h"
#include "G/GGameInstance.h"
#include "G/NonPlayerCharacter.h"
#include "G/Manager/CombatManager.h"
#include "Kismet/GameplayStatics.h"

bool USkillSpawnCharacter::CheckSkillPossibility()
{
	return Super::CheckSkillPossibility();
}

void USkillSpawnCharacter::SkillStart()
{
	Super::SkillStart();
}

void USkillSpawnCharacter::UseSkill()
{
	Super::UseSkill();

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FVector SpawnLocation = _myCharacter->GetActorLocation();
	float randomx = FMath::RandRange(50.0f, 100.0f);
	float randomy = FMath::RandRange(50.0f, 100.0f);
	SpawnLocation.X += randomx;
	SpawnLocation.Y += randomy;
	SpawnLocation.Z += randomy;
	FRotator rotator = _myCharacter->GetActorRotation();

	ANonPlayerCharacter* SpawnNPC = SpawnNPC = _myCharacter->GetWorld()->SpawnActor<ANonPlayerCharacter>(
		_spawnCharacter.Get(), SpawnLocation, rotator);

	UCombatManager* combatManager = GameInstance->GetCombatManager();

	SpawnNPC->SetCombatArea(combatManager->GetCombatArea());
	SpawnNPC->SetSpawnNPC(*_spawnCharacter.Get()->GetName());
	SpawnNPC->SetRewardGold(0);
	TArray<struct FRewardItemSetting> items;
	SpawnNPC->SetRewardItem(items);

	combatManager->AddNPCSpawn(SpawnNPC);
}

void USkillSpawnCharacter::SkillEnd()
{
	Super::SkillEnd();
}
