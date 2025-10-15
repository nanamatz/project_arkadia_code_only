// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/ManagerBase.h"
#include "G/Util/Define.h"
#include "CombatManager.generated.h"

/**
 * 
 */
UCLASS()
class G_API UCombatManager : public UManagerBase
{
	GENERATED_BODY()

	ECombatState _combatState;
	bool _isTurnEnd = false;
	bool bCombatTutorial = false;
	// 현재 전투지역
	UPROPERTY()
	class ACombatArea* _combatArea;
	int _curTurnCharacterNum;

	int _playerCount;
	int _npcCount;

	UPROPERTY()
	TSubclassOf<class ADamageTextActor> _damageTextActorClass;

	TArray<TPair<ECombatState, class AGCharacter*>> _turnList;

	UPROPERTY()
	class AGCharacter* _curTurnCharacter;
	
	FTimerHandle TurnTimerHandle;
	FTimerHandle RotateControlCharacterTimer;
	float _turnStartTime = 2.0f;

public:
	UCombatManager();

	virtual void Init(UGGameInstance* gameInstance) override;

	void OnDamageText(FVector location, int32 damage, EDamageType damageType);
	void OnCombatText(FVector location, FText text);

	
	void SetCombatState(ECombatState state);
	void SetCurTurnCharacter(AGCharacter* character);
	void SetTurnList();

	ECombatState GetCombatState() { return _combatState; }
	int GetCurTurnCharacterNum() { return _curTurnCharacterNum; }
	class ACombatArea* GetCombatArea() { return _combatArea; }
	
	class AGCharacter* GetCurTrunCharacter()
	{
		if (_curTurnCharacter)
		{
			return _curTurnCharacter;
		}
		return nullptr;
	}

	class AGCharacter* GetTrunCharacter(int idx)
	{
		if (_turnList.IsValidIndex(idx))
		{
			return _turnList[idx].Value;
		}
		return nullptr;
	}

	TPair<ECombatState, class AGCharacter*> GetFrontTurnList() { return _turnList[0]; }

	// false 면 비전투, true 면 전투 중
	bool CheckCombatTure()
	{
		if (!(CheckCombatState(CombatNone) || CheckCombatState(CombatEnd)))
		{
			//UE_LOG(LogTemp, Log, TEXT("CheckCombatTrue: Check Combat True"));
			return true;
		}

		//UE_LOG(LogTemp, Log, TEXT("CheckCombatTrue: Check Combat False"));
		return false;
	}

	bool CheckCombatState(ECombatState state)
	{
		if (_combatState == state)
		{
			return true;
		}
		return false;
	}

	void SetCombatArea(ACombatArea* combatArea);

	UFUNCTION(BlueprintCallable)
	void StartCombat(class ACombatArea* CombatArea);

	// 턴 큐 관리
	void AddTurnList(ECombatState CombatState, AGCharacter* character);

	void DeleteCharacterTurnList(class AGCharacter* deleteCharacter);

	void NextTurnQueue();
	void TurnStart();
	void TurnEnd();

	// 턴 관리
	UFUNCTION()
	void PlayerTurnStart();
	UFUNCTION()
	void PlayerTurnEnd(class AGCharacter* turnEndCharacter);

	UFUNCTION()
	void NPCTurnStart();
	UFUNCTION()
	void NPCTurnEnd(class AGCharacter* turnEndCharacter);
	UFUNCTION()
	void EndCombat();
	UFUNCTION()
	void EndGameConfirm();
	UFUNCTION()
	void AddNPCSpawn(class ANonPlayerCharacter* npc);
	UFUNCTION()
	void AddPlayerCharacter(class APlayerCharacter* playerCharacter);
	UFUNCTION()
	void AddTurnCharacter(class AGCharacter* character);

	UFUNCTION()
	void EndGame(bool bConfirm);
};
