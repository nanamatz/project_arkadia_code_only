// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatArea.generated.h"

/*
*
* Blueprint로 만든 CombatArea를 맵으로 끌어서 인스턴스를 만든다.
* Detail창에서 EnemyList를 수정한다.
* 
* Turn Queue로 턴 순서 관리
*/

USTRUCT()
struct FTurn
{
	GENERATED_USTRUCT_BODY()

	// BPlayerTurn이 True면 플레이어 턴, CharacterNum은 캐릭터 리스트에서 행동할 캐릭터 인덱스
	UPROPERTY()
	bool BPlayerTurn;
	UPROPERTY()
	int32 CharacterNum;
};

USTRUCT(BlueprintType)
struct FSpawnNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "CombatSet", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ANonPlayerCharacter> SpawnNPC;
	
	UPROPERTY(EditAnywhere, Category = "CombatSet", Meta = (AllowPrivateAccess = "true"))
	FVector SpawnLocation;

	// 확정 드랍 아이템들
	UPROPERTY(EditAnywhere, Category = "CombatSet", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UItemBase>> RewardItems;
};

USTRUCT(BlueprintType)
struct FRewardItemSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemBase> RewardItem;

	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	float DropRate = 50.0f;
};

UCLASS()
class G_API ACombatArea : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	class AGGameMode* gameMode;

	UPROPERTY()
	class UGGameInstance* gameInstance;

protected:
	bool BEnemyTurn;

	// 전투모드에 들어갔는지
	bool BCombatMode;

	// CombatArea에 있는 NPC가 생성 완료되었는지
	bool bSpawnNPC = false;

	// true - 게임 시작 시 생성하기
	UPROPERTY(EditAnywhere, Category = "CombatSet")
	bool bGameStartSpawnNPC = true;
	
	UPROPERTY(EditAnywhere, Category = "CombatSet", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class ANonPlayerCharacter>> CombatEnemyList;

	// 스폰 위치 세팅
	UPROPERTY(EditAnywhere, Category = "CombatSet", Meta = (AllowPrivateAccess = "true"))
	TArray<FVector> SpawnLocationList;
	
	UPROPERTY(EditAnywhere, Category = "CombatSet", Meta = (AllowPrivateAccess = "true"))
	TArray<struct FSpawnNPC> SpawnNPCList;
	
	UPROPERTY()
	TArray<ANonPlayerCharacter*> EnemyList;
	

	int32 EnemyCount;

	// 턴 큐
	TQueue<FTurn*> TurnQueue;

	// 현재 턴
	FTurn* CurTurn;

	UPROPERTY()
	class AGCharacter* curTurnCharacter;

	// 컨트롤하고 있는 캐릭터 번호
	int ControlEnemyNum = 0;

	// 해당 전투지역의 보상
	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UItemBase>> CombatRewardItems;

	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	TArray<FRewardItemSetting> CombatRewardItemList;
	
	UPROPERTY(EditAnywhere, Category = "Reward")
	int CombatRewardMinGold;

	UPROPERTY(EditAnywhere, Category = "Reward")
	int CombatRewardMaxGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNavigationInvokerComponent* NavigationInvokerComponent;
	
public:
	// Sets default values for this actor's properties
	ACombatArea();

protected:
	// Called when the game starts or when spawned
	/*virtual void BeginPlay() override;*/
	virtual void BeginPlay() override;
	
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
	UFUNCTION(CallInEditor)
	void UpdateSpawnLocationDebug();
	UFUNCTION(CallInEditor)
	void UpdateSpawnNPCList();
	UFUNCTION(CallInEditor)
	void UpdateRewardItemList();
#endif
	
	//virtual void PostRegisterAllComponents() override;
	virtual void BeginDestroy() override;
	
public:
	void SpawnCombatAreaNPC();
	
	class AGCharacter* GetCurTurnCharacter() { return curTurnCharacter; }
	int GetCombatRewardMinGold() { return CombatRewardMinGold; }
	int GetCombatRewardMaxGold() { return CombatRewardMaxGold; }
	TArray<TSubclassOf<class UItemBase>> GetCombatRewardItem() { return CombatRewardItems; }
	UFUNCTION(BluePrintCallable)
	TArray<class ANonPlayerCharacter*> GetEnemyList() { return EnemyList; }

	void SetBCombatMode(bool bValue) { BCombatMode = bValue; }

	void AddEnemyList(class ANonPlayerCharacter* Enemy);
};
