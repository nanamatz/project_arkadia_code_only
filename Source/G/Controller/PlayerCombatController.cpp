// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Controller/PlayerCombatController.h"

#include "G/G.h"
#include "G/GPlayerController.h"
#include "G/PlayerCharacter.h"
#include "G/Manager/InGameDataManager.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "G/UI/InGameUI/InGameUI.h"
#include "Kismet/GameplayStatics.h"

void UPlayerCombatController::SetPlayerInputState(EActionMode actionState, int cost)
{
	FString preState = UEnum::GetValueAsString(_playerInputState);
	FString curState = UEnum::GetValueAsString(actionState);

	UE_LOG(LogTemp, Warning, TEXT("preState : %s, curState : %s"), *preState, * curState);

	_playerInputState = actionState;

	switch (_playerInputState)
	{
	case Moving:
		SetStateMoving(cost);
		break;
	case BeforeActionSkill:
		SetStateBeforeSkillAction();
		break;
	case ActionSkill:
		SetStateActionSkill(cost);
		break;
	case ClickSkill:
		SetStateClickSkill();
		break;
	case ClickTarget:
		SetStateClickTarget();
		break;
	case ActionNone:
		SetStateActionNone();
		break;
	default:
		break;
	}
}

bool UPlayerCombatController::CheckPlayerInputState(EActionMode actionState)
{
	if (_playerInputState == actionState)
	{
		return true;
	}
	return false;
}

void UPlayerCombatController::SetStateActionNone()
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(GameInstance->GetWorld(), 0));
	PlayerController->SetDistanceWidgetVisibility(false);
	GameInstance->GetInGameDataManager()->GetControlCharacter()->OffViewAllSkillRange();
}

void UPlayerCombatController::SetStateBeforeSkillAction()
{
	APlayerCharacter* controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();

	if(!controlCharacter)
	{
		LOG(Warning, "Not Find Control Character");
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickTarget);
		controlCharacter->OffViewAllSkillRange();
		return;
	}
	USkillBase* mySkill = controlCharacter->GetSkillSystemComponent()->GetSelectedSkillSlot()->GetMySkill();
	
	// 타겟 체크
	AGCharacter* target = nullptr;
	ESkillTargetType skillTargetType = mySkill->GetSkillInfoManager()->GetTargetType();
	switch (skillTargetType)
	{
	case ESkillTargetType::SELF:
		target = controlCharacter;
		break;
	case ESkillTargetType::NONE:
		LOG(Error, "SkillTargetType None");
		break;
	default:
		target = GetTargetUnderCursor(controlCharacter->GetSelectedSkillTargetType());
	}

	if(target)
	{
		controlCharacter->GetSkillSystemComponent()->SetSkillTarget(target);
	}

	if(mySkill->GetSkillInfoManager()->GetSkillType() == ESkillType::TARGETING && !target){
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickTarget);
		return;
	}

	// 타겟 수 확인하고 SkillStart로 넘어가기
	if (controlCharacter->GetSkillSystemComponent()->CheckSkillPossibility())
	{
		controlCharacter->OffViewAllSkillRange();
		if (target)
		{
			controlCharacter->SkillAction(target);
		}
		else
		{
			controlCharacter->SkillAction(nullptr);
		}
	}
	else
	{
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickTarget);
	}
}

void UPlayerCombatController::SetStateClickSkill()
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(GameInstance->GetWorld(), 0));
	PlayerController->SetDistanceWidgetVisibility(true);
	GameInstance->GetInGameDataManager()->GetControlCharacter()->OffViewAllSkillRange();
}

void UPlayerCombatController::SetStateClickTarget()
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(GameInstance->GetWorld(), 0));
	PlayerController->SetDistanceWidgetVisibility(false);
	GameInstance->GetInGameDataManager()->GetControlCharacter()->OnViewAttackRange();
}

void UPlayerCombatController::SetStateActionSkill(int cost)
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(GameInstance->GetWorld(), 0));
	PlayerController->ChangeCursor(EMouseCursor::Default);
	SetPlayerInputState(ActionNone);
	UseCost(cost);
}

void UPlayerCombatController::SetStateMoving(int cost)
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(GameInstance->GetWorld(), 0));
	PlayerController->SetDistanceWidgetVisibility(false);
	UseCost(cost);
}

AGCharacter* UPlayerCombatController::GetTargetUnderCursor(ESkillTargetType TargetType)
{
	// 타겟으로 인식하는 것은 NPC만
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	if (TargetType == ESkillTargetType::NPC)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
	}
	else if (TargetType == ESkillTargetType::PLAYER)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}
	else if (TargetType == ESkillTargetType::ALL)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}

	APlayerCharacter* controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(controlCharacter->GetWorld(), 0);
	
	if (PlayerController)
	{
		UWorld* World = controlCharacter->GetWorld();
		FHitResult OutHitResult;

		if (PlayerController->GetHitResultUnderCursorForObjects(ObjectTypes, true, OutHitResult))
		{
			FVector HitLocation = OutHitResult.Location;
			AActor* HitActor = OutHitResult.GetActor();
			// 가져온 Actor가 AGCharacter 클래스의 인스턴스인지 확인
			if (HitActor && HitActor->IsA(AGCharacter::StaticClass()))
			{
				AGCharacter* TargetCharacter = Cast<AGCharacter>(HitActor);
				if (TargetCharacter && !TargetCharacter->GetBDeath())
				{
					// AGCharacter 클래스의 인스턴스를 반환
					return TargetCharacter;
				}
				if (TargetCharacter && TargetType == ESkillTargetType::DeadCharacter)
				{
					// AGCharacter 클래스의 인스턴스를 반환
					return TargetCharacter;
				}
			}
		}
	}
	else
	{
	}

	// 원하는 오브젝트를 찾지 못한 경우 nullptr 반환
	return nullptr;
}

void UPlayerCombatController::UseCost(int cost)
{
	if (cost <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PCC, UseCost"));
		return;
	}
	APlayerCharacter* controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	controlCharacter->AddCurrentCost(-cost);

	// UI 업데이트
	GameInstance->GetInGameUI()->SetCost(controlCharacter);
}


void UPlayerCombatController::SkillPreProcessing(int skillNum, USkillButton* target_button)
{
	APlayerCharacter* playerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();

	if (!playerCharacter)
	{
		return;
	}
	if (!playerCharacter->CheckUseSkill(skillNum))
	{
		return;
	}

	playerCharacter->GetSkillSystemComponent()->SetSelectedSkill(skillNum);
	USkillSlot* selectedSkillSlot = playerCharacter->GetSkillSystemComponent()->GetSelectedSkillSlot();

	if (!selectedSkillSlot)
	{
		return;
	}

	USkillBase* mySkill = selectedSkillSlot->GetMySkill();
	if (!mySkill)
	{
		return;
	}

	// 액션 모드 바꾸기
	SetPlayerInputState(ClickTarget);

	float AttackRange = mySkill->GetCharacterRange();
	int useCost = mySkill->GetCost();

	if (playerCharacter->GetCurrentCost() - useCost >= 0)
	{
		// 스킬 정보 가져오기

		playerCharacter->GetSkillSystemComponent()->SetSelectedSkill(skillNum);

		if (AttackRange > 0.01f)
		{
			// 사거리 표시
			playerCharacter->OnViewAttackRange();
		}
		else
		{
			// 즉발 처리
			GameInstance->GetPlayerCombatController()->SetPlayerInputState(BeforeActionSkill);
			//playerCharacter->SkillAction(nullptr);
		}
	}
	else
	{
	}
}

void UPlayerCombatController::HighlightTarget()
{
	ESkillRangeType skillRangetype = GameInstance->GetInGameDataManager()->GetControlCharacter()->GetSkillRangeType();

	switch (skillRangetype)
	{
	case ESkillRangeType::CHARACTERCIRCLE:
		HighlightSingleTarget();
		break;
	case ESkillRangeType::MOUSECIRCLE:
		HighlightMultiTarget();
		break;
	case ESkillRangeType::CHARACTERPIE:
		break;
	default:
		break;
	}
}

void UPlayerCombatController::HighlightSingleTarget()
{
	APlayerCharacter* controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	AGPlayerController* PlayerController = Cast<AGPlayerController>(
		UGameplayStatics::GetPlayerController(controlCharacter->GetWorld(), 0));

	ESkillTargetType targetType = GameInstance->GetInGameDataManager()->GetControlCharacter()->
	                                            GetSelectedSkillTargetType();

	// 타겟으로 인식하는 것은 NPC만
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	if (targetType == ESkillTargetType::NPC)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
	}
	else if (targetType == ESkillTargetType::PLAYER)
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}
	else
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(NPCChannel));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));
	}

	if (PlayerController)
	{
		UWorld* World = controlCharacter->GetWorld();
		FHitResult OutHitResult;

		if (PlayerController->GetHitResultUnderCursorForObjects(ObjectTypes, true, OutHitResult))
		{
			FVector HitLocation = OutHitResult.Location;
			AActor* HitActor = OutHitResult.GetActor();
			UE_LOG(LogTemp, Log, TEXT("playercombatcontroller 319 - hitactor name %s"), *(HitActor->GetName()));
			// 가져온 Actor가 AGCharacter 클래스의 인스턴스인지 확인
			if (HitActor && HitActor->IsA(AGCharacter::StaticClass()))
			{
				AGCharacter* TargetCharacter = Cast<AGCharacter>(HitActor);
				if (TargetCharacter && controlCharacter->GetSkillSystemComponent()->CheckTargetInAttackRange(
					TargetCharacter))
				{
					if (!TargetCharacter->GetBDeath())
					{
						UE_LOG(LogTemp, Log, TEXT("playercombatcontroller 329 - hitactor name %s"), *(HitActor->GetName()));
						PlayerController->ChangeSelectedCursorTarget_Temp(HitActor);
					}

					if (targetType == ESkillTargetType::DeadCharacter && TargetCharacter->GetBDeath())
					{
						UE_LOG(LogTemp, Log, TEXT("playercombatcontroller 335 - hitactor name %s"), *(HitActor->GetName()));
						PlayerController->ChangeSelectedCursorTarget_Temp(HitActor);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("playercombatcontroller 343 - nullptr"));
			PlayerController->ChangeSelectedCursorTarget_Temp(nullptr);
		}
	}
}

void UPlayerCombatController::HighlightMultiTarget()
{
	// 사거리 표시 액터에서 충돌 처리로 하이라이트
}
