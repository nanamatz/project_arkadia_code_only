// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Manager/CombatManager.h"

#include "InGameDataManager.h"
#include "G/CombatArea.h"
#include "G/GGameInstance.h"
#include "G/NonPlayerCharacter.h"
#include "G/PlayerCharacter.h"
#include "G/UI/InGameUI/InGameUI.h"
#include <G/Util/Define.h>
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "InventoryManager.h"
#include "ResourceManager.h"
#include "TutorialManager.h"
#include "G/G.h"
#include "G/GPlayerCamera.h"
#include "G/NPCAIController.h"
#include "G/Controller/PlayerCombatController.h"
#include "G/UI/TurnQueueUI/TurnQueueWidget.h"
#include "G/GPlayerController.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "G/UI/ConfirmationBox.h"
#include "G/UI/DamageTextActor.h"
#include "G/UI/GameMenuManager.h"

UCombatManager::UCombatManager()
{
	_combatState = CombatNone;
	
}

void UCombatManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
	_combatState = CombatNone;
	_combatArea = nullptr;
	_npcCount = 0;
	_playerCount = 0;
	_turnList.Reset();
	_curTurnCharacter = nullptr;
	_isTurnEnd = false;
	_curTurnCharacterNum = 0;
	_damageTextActorClass = ADamageTextActor::StaticClass();
}

void UCombatManager::OnDamageText(FVector location, int32 damage, EDamageType damageType = Default)
{
	ADamageTextActor* damageTextActor = GameInstance->GetResourceManager()->Instantiate(_damageTextActorClass);
	LOG(Log, "damageTextActor : %s", *damageTextActor->GetName());
	damageTextActor->Init(location, damage, damageType);
}

void UCombatManager::OnCombatText(FVector location, FText text)
{
	ADamageTextActor* damageTextActor = GameInstance->GetResourceManager()->Instantiate(_damageTextActorClass);
	damageTextActor->Init(location, text);
}

void UCombatManager::SetCombatState(ECombatState state)
{
	_combatState = state;
}


void UCombatManager::SetCurTurnCharacter(AGCharacter* character)
{
	_curTurnCharacter = character;
}

void UCombatManager::SetTurnList()
{
	if (_combatState != CombatStart)
	{
		return;
	}
	TArray<APlayerCharacter*> playerCharacters = GameInstance->GetInGameDataManager()->GetPlayerCharacterList();
	TArray<ANonPlayerCharacter*> nonPlayerCharacters = _combatArea->GetEnemyList();

	int playerCharactersNum = playerCharacters.Num();
	int nonPlayerCharactersNum = nonPlayerCharacters.Num();
	_playerCount = 0;
	_npcCount = 0;
	int playerIndex = 0;
	int npcIndex = 0;
	
	int charactersSum = playerCharactersNum + nonPlayerCharactersNum;

	for (int i = 0; i < charactersSum; i++)
	{
		if (_playerCount < playerCharactersNum)
		{
			for(; playerIndex < playerCharactersNum; playerIndex++)
			{
				if(!playerCharacters[playerIndex]->GetBDeath()) break;
			}
			if(playerCharacters.IsValidIndex(playerIndex))
			{
				AddTurnList(PlayerTurn, playerCharacters[playerIndex++]);
			}
		}
		if (_npcCount < nonPlayerCharactersNum)
		{
			for(; npcIndex < nonPlayerCharactersNum; npcIndex++)
			{
				if(!nonPlayerCharacters[npcIndex]->GetBDeath()) break;
			}
			if(nonPlayerCharacters.IsValidIndex(npcIndex))
			{
				AddTurnList(NPCTurn, nonPlayerCharacters[npcIndex++]);
			}
		}
	}

	GameInstance->GetInGameUI()->GetTurnQueueUI()->On();
}

void UCombatManager::AddNPCSpawn(ANonPlayerCharacter* npc)
{
	ANPCAIController* aiController = Cast<ANPCAIController>(npc->Controller);
	aiController->SetBCombat(true);
	AddTurnList(NPCTurn, npc);
}

void UCombatManager::AddPlayerCharacter(APlayerCharacter* playerCharacter)
{
	_playerCount++;
	AddTurnList(PlayerTurn, playerCharacter);
}

void UCombatManager::AddTurnCharacter(AGCharacter* character)
{
	if (character->GetECharacterName() != ECharacterName::NonPlayerCharacter && !character->GetBDeath())
	{
		_playerCount++;
		AddTurnList(PlayerTurn, character);
	}
	else
	{
		ANPCAIController* aiController = Cast<ANPCAIController>(character->Controller);
		aiController->SetBCombat(true);
		_npcCount++;
		AddTurnList(NPCTurn, character);
	}
}


void UCombatManager::AddTurnList(ECombatState CombatState, AGCharacter* character)
{
	if (character->GetBDeath())
	{
		return;
	}

	switch (CombatState)
	{
	case PlayerTurn:
		_playerCount++;
		break;
	case NPCTurn:
		_npcCount++;
		break;
	default:
		// error
		break;
	}

	character->SetAffectNavigation(true);
	_turnList.Add(TPair<ECombatState, AGCharacter*>(CombatState, character));
	GameInstance->GetInGameUI()->GetTurnQueueUI()->InsertPortrait(character);
}

void UCombatManager::DeleteCharacterTurnList(AGCharacter* deleteCharacter)
{
	int idx = 0;

	for (auto curTurn : _turnList)
	{
		if (curTurn.Value == deleteCharacter)
		{
			if (curTurn.Key == PlayerTurn)
			{
				_playerCount--;
				if (_playerCount == 0)
				{
				}
			}
			if (curTurn.Key == NPCTurn)
			{
				_npcCount--;
			}

			if (_curTurnCharacter == curTurn.Value)
			{
				NextTurnQueue();
			}

			_turnList.RemoveAt(idx);
			GameInstance->GetInGameUI()->GetTurnQueueUI()->DeletePortrait(idx);

			UE_LOG(LogTemp, Log, TEXT("DELETECHARACTERTURNLIST, _playerCount:%d"), _playerCount);

			if (_playerCount == 0)
			{
				EndGameConfirm();
				return;
			}

			UE_LOG(LogTemp, Log, TEXT("DELETECHARACTERTURNLIST, _npcCount:%d"), _npcCount);
			if (_npcCount == 0)
			{
				EndCombat();
				return;
			}

			return;
		}
		idx++;
	}
}


void UCombatManager::SetCombatArea(ACombatArea* combatArea)
{
	_combatArea = combatArea;
	if (_combatArea)
	{
		_combatArea->SetBCombatMode(true);
	}
}

void UCombatManager::StartCombat(ACombatArea* combatArea)
{
	combatArea->SpawnCombatAreaNPC();
	if (CheckCombatTure())
	{
		return;
	}
	
	GameInstance->GetInGameUI()->GetTurnQueueUI()->ClearPortrait();
	GameInstance->GetInGameUI()->OnSkillBox();
	GameInstance->GetInGameUI()->GetTurnQueueUI()->AnnouncementStartCombat();

	// 플레이어 캐릭터 일시정지
	TArray<APlayerCharacter*> playerCharacters = GameInstance->GetInGameDataManager()->GetPlayerCharacterList();
	for (APlayerCharacter* playerCharacter : playerCharacters)
	{
		AController* Controller = playerCharacter->GetController();
		if (Controller)
		{
			Controller->StopMovement();
		}
	}

	APlayerController* apc = UGameplayStatics::GetPlayerController(combatArea->GetWorld(), 0);
	AGPlayerController* GController = Cast<AGPlayerController>(apc);
	GController->SetInputState(EInputState::State::Combat);

	SetCombatState(CombatStart);
	SetCombatArea(combatArea);
	SetTurnList();

	// 플레이어 캐릭터 방향 보도록 회전
	for (class ANonPlayerCharacter* Enemy : _combatArea->GetEnemyList())
	{
		if (playerCharacters.IsValidIndex(0))
		{
			Enemy->RotateTowardTarget(playerCharacters[0]);

			ANPCAIController* aiController = Cast<ANPCAIController>(Enemy->Controller);
			aiController->SetBCombat(true);
		}
	}

	NextTurnQueue();
}

void UCombatManager::NextTurnQueue()
{
	if (CheckCombatState(CombatEnd))
	{
		GameInstance->GetInGameUI()->GetTurnQueueUI()->ClearPortrait();
		return;
	}

	SetCombatState(BeforeTurnStart);
	_combatArea->GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	_combatArea->GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &UCombatManager::TurnStart, _turnStartTime);
}

void UCombatManager::TurnStart()
{
	if (!CheckCombatState(BeforeTurnStart) || CheckCombatState(CombatEnd))
	{
		return;
	}
	SetCurTurnCharacter(GetFrontTurnList().Value);
	SetCombatState(GetFrontTurnList().Key);

	AGCharacter* turnCharacter = Cast<AGCharacter>(_curTurnCharacter);

	if (turnCharacter && turnCharacter->CheckDebuff(EDEBUFFTYPE::STUN))
	{
		turnCharacter->CharacterCombatTurnStart();
		TurnEnd();
		return;
	}

	_curTurnCharacter->CharacterCombatTurnStart();

	GameInstance->GetInGameUI()->GetTurnQueueUI()->EmphasisPortrait();
	GameInstance->GetInGameUI()->SetCost(turnCharacter);
	GameInstance->GetInGameUI()->SetSkillButton(turnCharacter->GetSkillSystemComponent()->GetSkillSlotList());
	GameInstance->GetInGameUI()->OnSkillBox();
	GameInstance->GetInGameUI()->OnCost();
	
	// 카메라 회전
	AGPlayerCamera* camera = Cast<AGPlayerCamera>(UGameplayStatics::GetPlayerPawn(turnCharacter->GetWorld(), 0));
	if(camera && turnCharacter)
	{
		float deltaSec = turnCharacter->GetWorld()->GetDeltaSeconds();
		turnCharacter->GetWorldTimerManager().SetTimer(RotateControlCharacterTimer, [this, camera, turnCharacter]
		{
			if(camera->RotateControlCharacterCamera(turnCharacter))
			{
				switch (_combatState)
				{
				case PlayerTurn:
					PlayerTurnStart();
					break;
				case NPCTurn:
					NPCTurnStart();
					break;
				default:
					break;
				}
				turnCharacter->GetWorldTimerManager().ClearTimer(RotateControlCharacterTimer);
			}
		},deltaSec, true);
	}
}

void UCombatManager::TurnEnd()
{
	if (_isTurnEnd || CheckCombatState(BeforeTurnStart) || CheckCombatState(CombatEnd))
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("combat State : %s"), *UEnum::GetValueAsString(GetCombatState()));

	_isTurnEnd = true;

	for (auto& curTurn : _turnList)
	{
	}

	switch (_combatState)
	{
	case PlayerTurn:
		PlayerTurnEnd(_curTurnCharacter);
		break;
	case NPCTurn:
		NPCTurnEnd(_curTurnCharacter);
		break;
	default:
		return;
	}


	auto frontTurn = GetFrontTurnList();
	_turnList.Remove(frontTurn);
	_turnList.Add(frontTurn);

	GameInstance->GetInGameUI()->GetTurnQueueUI()->FirstToLastPortrait();
	_curTurnCharacter->CharacterCombatTurnEnd();

	NextTurnQueue();
	_isTurnEnd = false;
}

void UCombatManager::PlayerTurnStart()
{
	if(!bCombatTutorial)
	{
		GameInstance->GetTutorialManager()->PlayTutorial(ETutorial::Combat);
		bCombatTutorial = true;
	}
	if (CheckCombatState(CombatEnd))
	{
		return;
	}

	if (_curTurnCharacter->GetBDeath())
	{
		NextTurnQueue();
	}

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(_curTurnCharacter);
	if (playerCharacter)
	{
		GameInstance->GetInGameUI()->StartPlayerTurn(playerCharacter);

		GameInstance->GetInGameDataManager()->SetControlCharacter(playerCharacter);
		playerCharacter->OnSelectCharacter();

		// 스킬 고르기
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickSkill);
	}
	else
	{
		NextTurnQueue();
	}
}

void UCombatManager::PlayerTurnEnd(class AGCharacter* turnEndCharacter)
{
	if (CheckCombatState(CombatEnd))
	{
		return;
	}
	
	GameInstance->GetPlayerCombatController()->SetPlayerInputState(ActionNone);
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(turnEndCharacter);
	if (playerCharacter)
	{
		GameInstance->GetInGameUI()->EndTurn();
		playerCharacter->FillCost();
		playerCharacter->OffSelectCharacter();
	}
}

void UCombatManager::NPCTurnStart()
{
	if (CheckCombatState(CombatEnd))
	{
		return;
	}

	if (_curTurnCharacter->GetBDeath())
	{
		NextTurnQueue();
	}

	ANonPlayerCharacter* npc = Cast<ANonPlayerCharacter>(_curTurnCharacter);
	if (npc)
	{
		npc->SetBControlling(true);
		npc->FillCost();
	}
	else
	{
		NextTurnQueue();
	}
}

void UCombatManager::NPCTurnEnd(class AGCharacter* turnEndCharacter)
{
	if (CheckCombatState(CombatEnd))
	{
		return;
	}

	ANonPlayerCharacter* npc = Cast<ANonPlayerCharacter>(turnEndCharacter);
	if (npc)
	{
		npc->SetBControlling(false);
	}
}

void UCombatManager::EndCombat()
{
	if(_combatState == ECombatState::CombatNone)
	{
		return;
	}
	
	if (_combatState == CombatEnd)
	{
		UE_LOG(LogTemp, Log, TEXT("CombatManager:EndCombat _combatState == CombatEnd so return"));
		return;
	}

	// 전투 종료 처리
	for (class ANonPlayerCharacter* Enemy : _combatArea->GetEnemyList())
	{
		ANPCAIController* aiController = Cast<ANPCAIController>(Enemy->Controller);
		aiController->SetBCombat(false);
	}
	
	
	UInGameUI* InGameUI = GameInstance->GetInGameUI();

	InGameUI->GetTurnQueueUI()->AnnouncementEndCombat();
	InGameUI->OffCost();

	SetCombatState(CombatEnd);
	_turnList.Empty();

	APlayerController* apc = UGameplayStatics::GetPlayerController(_combatArea->GetWorld(), 0);
	AGPlayerController* GController = Cast<AGPlayerController>(apc);
	GController->SetInputState(EInputState::State::Default);

	InGameUI->EndCombat();
	
	GameInstance->GetPlayerCombatController()->SetPlayerInputState(ActionNone);

	SetCurTurnCharacter(nullptr);
	
	_combatArea->GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	_combatArea->GetWorldTimerManager().SetTimer(TurnTimerHandle, [this]()
	{
		UE_LOG(LogTemp, Log, TEXT("CombatManger: Timer Lambda Function Called, playerCharacter Init"));
		for (APlayerCharacter* playerCharacter : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
		{
			playerCharacter->InitPlayerCharacter();
		}
	}, _turnStartTime, false);
}

void UCombatManager::EndGameConfirm()
{
	if (_combatState == CombatEnd)
	{
		return;
	}
	
	EndCombat();
	
	auto confirmBox = GameInstance->GetConfirmBox();

	confirmBox->On(FText::FText(NSLOCTEXT("GameOverConfirmBox", "Arakadia", "게임 오버")), false);
	
	// 델리게이트에 함수 바인딩
	confirmBox->OnConfirmResult.AddDynamic(this, &UCombatManager::EndGame);
}

void UCombatManager::EndGame(bool bConfirm)
{
	EndCombat();
	
	if (bConfirm)
	{
		GameInstance->GetInventoryManager()->ClearInventory();

		GameInstance->GetGameMenuManager()->Clear();
		GameInstance->GetInGameUI()->ClearPortrait();
		GameInstance->GetInGameUI()->GetTurnQueueUI()->Off();
		
		SetCombatArea(nullptr);

		GameInstance->LoadMenuLevel();
		
		UE_LOG(LogTemp, Log, TEXT("End Game!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Cancelled!"));
	}
}