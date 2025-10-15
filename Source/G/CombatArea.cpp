// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatArea.h"

#include "DrawDebugHelpers.h"
#include "G.h"
#include "NavigationInvokerComponent.h"
#include "NPCAIController.h"
#include "G/NonPlayerCharacter.h"


// Sets default values
ACombatArea::ACombatArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComponent"));
}

void ACombatArea::BeginPlay()
{
	Super::BeginPlay();

	if(bGameStartSpawnNPC)
	{
		SpawnCombatAreaNPC();
	}
}

void ACombatArea::SpawnCombatAreaNPC()
{
	if(bSpawnNPC)
	{
		return;
	}
	bSpawnNPC = true;
	for (int i = 0; i < SpawnNPCList.Num(); i++)
	{
		TSubclassOf<ANonPlayerCharacter> SpawnNPC = SpawnNPCList[i].SpawnNPC;

		if(!SpawnNPC)
		{
			LOG(Warning, "%s : %d npc null", *GetName(), i);
			continue;
		}
		FVector SpawnLocation = SpawnNPCList[i].SpawnLocation;

		UClass* NPCClassPtr = SpawnNPC.Get(); 
		// UClass를 사용하여 블루프린트 액터를 스폰한다.
		SpawnLocation.Z += 150.0f;
		ANonPlayerCharacter* NPC = GetWorld()->SpawnActor<ANonPlayerCharacter>(NPCClassPtr, SpawnLocation, FRotator::ZeroRotator);

		if(!NPC)
		{
			/*/*LOG(Warning, "%s Fail NPC Spawn", *SpawnLocation.ToString());#1#/#1#*/
			continue;
		}

		NPC->DispatchBeginPlay(true);

		// 정보 세팅
		LOG(Log, "Combat Area Nane : %s", *GetName());
		NPC->SetCombatArea(this);
		NPC->SetSpawnNPC(*NPC->GetClass()->GetName());
		NPC->SetSpawnLocation(SpawnLocation);
		
		AddEnemyList(NPC);
		
		// 골드 세팅
		int MinGold = GetCombatRewardMinGold();
		int MaxGold = GetCombatRewardMaxGold();
		int RandomRewardGold = FMath::RandRange(MinGold, MaxGold);
		NPC->SetRewardGold(RandomRewardGold);
		// 아이템 세팅
		if(!SpawnNPCList[i].RewardItems.IsEmpty())
		{
			for(auto item : SpawnNPCList[i].RewardItems)
			{
				NPC->AddRewardItem(item);
			}
		}
		NPC->SetRewardItem(CombatRewardItemList);
	}
}


#if WITH_EDITOR

// Called when a property is changed in the editor (including nested properties like SpawnLocation)
void ACombatArea::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	// 변경된 프로퍼티가 SpawnNPCList일 때만 처리
	
	FName propertyName = PropertyChangedEvent.GetPropertyName();
	if (propertyName == GET_MEMBER_NAME_CHECKED(ACombatArea, SpawnNPCList) ||
		propertyName.ToString() == "X" ||
		propertyName.ToString() == "Y" ||
		propertyName.ToString() == "Z" )
	{
		// 액터의 위치를 가져옴
		FVector ActorLocation = GetActorLocation();

		// SpawnNPCList의 각 요소에 대해 확인
		for (FSpawnNPC& SpawnNPC : SpawnNPCList)
		{
			// SpawnLocation이 기본값일 경우 Actor의 위치로 설정
			if (SpawnNPC.SpawnLocation.IsZero())
			{
				SpawnNPC.SpawnLocation = ActorLocation;
			}
			
			UpdateSpawnLocationDebug();
		}
	}
}

void ACombatArea::UpdateSpawnLocationDebug()
{
	// 모든 스폰 위치에 대해 디버그 표시를 업데이트
	FVector ActorLocation = GetActorLocation();

	for (const FSpawnNPC& SpawnNPC : SpawnNPCList)
	{
		// SpawnLocation이 기본값이 아니면 디버그 구와 선을 그리기
		if (!SpawnNPC.SpawnLocation.IsZero())
		{
			// 디버그 구 그리기
			DrawDebugSphere(GetWorld(), SpawnNPC.SpawnLocation, 50.0f, 12, FColor::Red, false, 5.0f, 0, 2.0f);

			// 액터의 위치에서 SpawnLocation까지의 선 그리기
			DrawDebugLine(GetWorld(), ActorLocation, SpawnNPC.SpawnLocation, FColor::Green, false, 5.0f, 0, 2.0f);
		}
	}
}

void ACombatArea::UpdateSpawnNPCList()
{
	if(SpawnNPCList.Num() == 0 && CombatEnemyList.Num() != 0)
	{
		for(int i = 0; (i < CombatEnemyList.Num() && CombatEnemyList.IsValidIndex(i) && SpawnLocationList.IsValidIndex(i)); i++)
		{
			FSpawnNPC newNPC;
			newNPC.SpawnNPC = CombatEnemyList[i];
			newNPC.SpawnLocation = SpawnLocationList[i];
			SpawnNPCList.Add(newNPC);	
		}
	}
}

void ACombatArea::UpdateRewardItemList()
{
	if(CombatRewardItemList.Num() == 0 && CombatRewardItems.Num() != 0)
	{
		for(auto Item : CombatRewardItems)
		{
			FRewardItemSetting newItemSetting;
			newItemSetting.RewardItem = Item;
			CombatRewardItemList.Add(newItemSetting);
		}
	}
}
#endif


void ACombatArea::BeginDestroy()
{
	Super::BeginDestroy();

	for(auto enemy : EnemyList)
	{
		if(enemy)
		{
			enemy->Destroy();
		}
	}
}

void ACombatArea::AddEnemyList(ANonPlayerCharacter* Enemy)
{
	EnemyList.Add(Enemy);
	EnemyCount++;
}