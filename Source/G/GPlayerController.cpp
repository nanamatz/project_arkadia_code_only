// Copyright Epic Games, Inc. All Rights Reserved.

#include "GPlayerController.h"

#include "GGameInstance.h"
#include "GGameMode.h"
#include "CharacterAIController.h"
#include "G.h"
#include "PlayerCharacter.h"
#include "G/Dialog/InteractionCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Controller/PlayerCombatController.h"
#include "Controller/PlayerMoveController.h"
#include "G/UI/GameMenuManager.h"
#include "Manager/CombatManager.h"
#include "UI/InGameUI/InGameUI.h"
#include "G/UI/Looting/LootingActor.h"
#include "GPlayerCamera.h"
#include "G/NonPlayerCharacter.h"
#include "G/Dialog/DialogComponentBase.h"
#include "G/Dialog/DialogWidgetBase.h"
#include "G/Dialog/DialogManageActor.h"
#include "G/UI/InteractionIcon.h"
#include "Manager/FXManager.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "UI/InGameUI/ESCMenuWidget.h"
#include "G/Manager/TutorialManager.h"

AGPlayerController::AGPlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;

	// Set up the class finder in the constructor
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(
		TEXT("WidgetBlueprint'/Game/UI/GameUI/BP_DistanceTextWidget.BP_DistanceTextWidget_C'"));
	if (WidgetClassFinder.Succeeded())
	{
		DistanceTextWidgetClass = WidgetClassFinder.Class;
	}
	else
	{
		// Handle failure to find the widget class
		DistanceTextWidgetClass = nullptr;
	}
	
	_cursorCharacterTarget = nullptr;
}

void AGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	this->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));

	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	_combatManager = GameInstance->GetCombatManager();

	if (DistanceTextWidgetClass)
	{
		DistanceTextWidget = CreateWidget<UUserWidget>(this, DistanceTextWidgetClass);
		if (DistanceTextWidget)
		{
			DistanceTextWidget->AddToViewport();
			SetDistanceWidgetVisibility(false);
		}
	}
}

void AGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (_combatManager->CheckCombatTure())
	{
		CombatPlayerTick();
	}
	else
	{
		DefaultPlayerTick_Temp();
	}

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}

	if ((GameInstance->GetInputState() != EInputState::State::Dialog) 
		&& (GameInstance->GetInputState() != EInputState::State::UI)
		&& !(_combatManager->CheckCombatTure()))
	{
		APlayerCharacter* ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
		if (ControlCharacter)
		{
			if (ControlCharacter->GetInteractableinRange().Num() > 0)
			{
				UDialogComponentBase* dcb = Cast<UDialogComponentBase>(ControlCharacter->GetInteractableinRange()[0]);
				if (dcb)
				{
					if (dcb->GetOwnerActor()->GetOwnerChar()->GetQuestVisible())
					{
						FVector2D ScreenLocation;
						FVector targetlocation = dcb->GetOwnerActor()->GetOwnerChar()->GetActorLocation();
						this->ProjectWorldLocationToScreen(targetlocation, ScreenLocation);
						GameInstance->GetGameMenuManager()->GetInteractionIcon()->OnInteractIcon(ScreenLocation, dcb->GetOwnerActor()->GetOwnerChar()->InteractIconOffset);
					}
				}
			}
			else
			{
				GameInstance->GetGameMenuManager()->GetInteractionIcon()->OffInteractIcon();
			}
		}
	}
	else if ((GameInstance->GetInputState() == EInputState::State::Dialog) 
		|| (GameInstance->GetInputState() == EInputState::State::UI)
		|| _combatManager->CheckCombatTure())
	{
		GameInstance->GetGameMenuManager()->GetInteractionIcon()->OffInteractIcon();
	}

	if (IsGoingLootingTarget)
	{
		APlayerCharacter* ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();

		if (_combatManager->CheckCombatTure())
		{
			ControlCharacter = Cast<APlayerCharacter>(_combatManager->GetCurTrunCharacter());
		}

		if(IsValid(ControlCharacter)) {
			if (Distance_LootingTarget > ControlCharacter->GetDistanceTo(GoingLootingTarget))
			{
				FVector2D ScreenLocation;
				SetNewMoveDestination(ControlCharacter->GetActorLocation());
				ALootingActor* lat = Cast<ALootingActor>(GoingLootingTarget);
				ANonPlayerCharacter* nplat = Cast<ANonPlayerCharacter>(GoingLootingTarget);
				if (lat)
				{
					this->ProjectWorldLocationToScreen(lat->GetActorLocation(), ScreenLocation);
					lat->OpenLootingUI(ScreenLocation);
				}
				else if (nplat)
				{
					this->ProjectWorldLocationToScreen(nplat->GetActorLocation(), ScreenLocation);
					nplat->OpenLootingUI(ScreenLocation);
				}
				LootingUIOn = true;
				GetMousePosition(LootingUIPosition.X, LootingUIPosition.Y);
				IsGoingLootingTarget = false;
				GoingLootingTarget = nullptr;
			}
		}
	}
}

void AGPlayerController::DefaultPlayerTick()
{
	// 타겟으로 인식하는 것은 NPC만
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(PLAYERChannel));

	UWorld* World = GetWorld();
	FHitResult OutHitResult;

	if (GetHitResultUnderCursorForObjects(ObjectTypes, true, OutHitResult))
	{
		AActor* HitActor = OutHitResult.GetActor();
		// 가져온 Actor가 AGCharacter 클래스의 인스턴스인지 확인
		if (HitActor)
		{
			AGCharacter* TargetCharacter = Cast<AGCharacter>(HitActor);
			if (TargetCharacter && TargetCharacter->GetBDeath())
			{
				ChangeCursor(EMouseCursor::CardinalCross);
				ChangeSelectedCursorTarget(TargetCharacter);
			}
		}
	}
	else
	{
		ChangeCursor(EMouseCursor::Default);
		ChangeSelectedCursorTarget(nullptr);
	}
}

void AGPlayerController::DefaultPlayerTick_Temp()
{
	//UE_LOG(LogTemp, Log, TEXT("AGPlyaerController (121) InteracionCharTick Execute"));

	// 타겟으로 인식하는 것은 NPC와 인터렉트채널
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(InteractChannel));

	UWorld* World = GetWorld();
	FHitResult OutHitResult;

	if (GetHitResultUnderCursorForObjects(ObjectTypes, true, OutHitResult))
	{
		AActor* HitActor = OutHitResult.GetActor();
		//LOG(Log, "Hit Actor : %s", *AActor::GetDebugName(HitActor));
		// 가져온 Actor가 유효한지 확인
		if (HitActor->IsA(AGCharacter::StaticClass()) ||
			HitActor->IsA(ALootingActor::StaticClass()) ||
			HitActor->IsA(AInteractionCharacter::StaticClass()))
		{
			ChangeSelectedCursorTarget_Temp(HitActor);
		}
		else
		{
			ChangeSelectedCursorTarget_Temp(nullptr);
		}
	}
	else
	{
		ChangeSelectedCursorTarget_Temp(nullptr);
	}
}

void AGPlayerController::CombatPlayerTick()
{
	if (_combatManager->CheckCombatState(PlayerTurn))
	{
		if (GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickTarget))
		{
			GameInstance->GetPlayerCombatController()->HighlightTarget();
		}

		if (GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickSkill))
		{
			DrawPath();

			if (GetMousePointNavDistance(MoveDistance))
			{
				UpdateDistanceText(MoveDistance);
			}
			
			//적 LootingUI
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(InteractChannel));

			UWorld* World = GetWorld();
			FHitResult OutHitResult;

			if (GetHitResultUnderCursorForObjects(ObjectTypes, true, OutHitResult))
			{
				AActor* HitActor = OutHitResult.GetActor();
				// 가져온 Actor가 유효한지 확인
				if (HitActor)
				{
					ANonPlayerCharacter* nontarget = Cast<ANonPlayerCharacter>(HitActor);
					if (nontarget && nontarget->GetBDeath())
					{
						ChangeSelectedCursorTarget_Temp(HitActor);
					}
					else
					{
						ChangeSelectedCursorTarget_Temp(nullptr);
					}
				}
			}
			else
			{
				ChangeSelectedCursorTarget_Temp(nullptr);
			}
		}

		if (GameInstance->GetPlayerCombatController()->CheckPlayerInputState(Moving) &&
			!IsMoveInputIgnored() && (GameInstance->GetInGameDataManager()->GetControlCharacter()->GetVelocity().
			                                        SizeSquared() < FMath::Square(1.0f)))
		{
			StopMovement();
			GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickSkill);
		}
	}
}

void AGPlayerController::OnCheat()
{
	GameInstance->GetInGameDataManager()->GetControlCharacter()->GetCharacterInfoManager()->LevelUp();
	// 예시: 게임의 전체 시간을 2배로 빠르게 설정
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 3.0f);
}

void AGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, int8);

	DECLARE_DELEGATE_OneParam(FGameMenuUIInputDelegate, UI_TYPE);

	InputComponent->BindAction<FCustomInputDelegate>("CharacterChange_1", IE_Pressed, this,
	                                                 &AGPlayerController::OnCharacterChange, static_cast<int8>(1));
	InputComponent->BindAction<FCustomInputDelegate>("CharacterChange_2", IE_Pressed, this,
	                                                 &AGPlayerController::OnCharacterChange, static_cast<int8>(2));
	InputComponent->BindAction<FCustomInputDelegate>("CharacterChange_3", IE_Pressed, this,
	                                                 &AGPlayerController::OnCharacterChange, static_cast<int8>(3));
	InputComponent->BindAction<FCustomInputDelegate>("CharacterChange_4", IE_Pressed, this,
	                                                 &AGPlayerController::OnCharacterChange, static_cast<int8>(4));

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AGPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AGPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("OnClickTargetAttack", IE_Released, this, &AGPlayerController::OnClickUseSkill);
	InputComponent->BindAction("OnClickTargetAttack", IE_Pressed, this, &AGPlayerController::OnLMBPressed);

	InputComponent->BindAction("OnClickCancelAction", IE_Released, this, &AGPlayerController::OnClickCancelAction);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AGPlayerController::OnInteract);

	InputComponent->BindAction<FGameMenuUIInputDelegate>("InventoryUI", IE_Pressed, this,
	                                                     &AGPlayerController::ControlGameMenuUI, UI_TYPE::Inventory);
	InputComponent->BindAction<FGameMenuUIInputDelegate>("SkillUI", IE_Pressed, this,
	                                                     &AGPlayerController::ControlGameMenuUI, UI_TYPE::Skill);
	InputComponent->BindAction<FGameMenuUIInputDelegate>("QuestUI", IE_Pressed, this,
	                                                     &AGPlayerController::ControlGameMenuUI, UI_TYPE::Quest);
	InputComponent->BindAction<FGameMenuUIInputDelegate>("MapUI", IE_Pressed, this,
	                                                     &AGPlayerController::ControlGameMenuUI, UI_TYPE::Map);

	InputComponent->BindAction("DoubleGameSpeed", IE_Pressed, this, &AGPlayerController::SetDoubleGameSpeed);
	InputComponent->BindAction("DoubleGameSpeed", IE_Released, this, &AGPlayerController::SetDefaultGameSpeed);
	InputComponent->BindAction("ESCMenu", IE_Pressed, this, &AGPlayerController::OnESCMenu);

	InputComponent->BindAction("DialogAccept", IE_Pressed, this, &AGPlayerController::OnDialogAccept);
	InputComponent->BindAction("DialogChooseUp", IE_Pressed, this, &AGPlayerController::OnDialogChooseUp);
	InputComponent->BindAction("DialogChooseDown", IE_Pressed, this, &AGPlayerController::OnDialogChooseDown);
	InputComponent->BindAction("DialogAcceptMouse", IE_Pressed, this, &AGPlayerController::OnDialogAcceptMouse);

	InputComponent->BindAction("Cheat", IE_Pressed, this, &AGPlayerController::OnCheat);
}

void AGPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;

	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (GameInstance->GetCombatManager()->CheckCombatTure() &&
		Hit.bBlockingHit &&
		GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickSkill))
	{
		SetNewMoveDestination(Hit.ImpactPoint);
	}
	else if (!GameInstance->GetCombatManager()->CheckCombatTure()
		&& Hit.bBlockingHit)
	{
		// 클릭한 위치의 월드 좌표
		FVector ClickLocation = Hit.ImpactPoint;
		// 클릭한 위치에 파티클 시스템을 한 번 스폰
		mouseCurTime += GetWorld()->GetDeltaSeconds();
		if(mouseVFXtime < mouseCurTime)
		{
			mouseCurTime = 0;
			GameInstance->GetFXManager()->PlayVFX(EVFXType::MouseCursor, ClickLocation);
		}
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}


void AGPlayerController::SetNewMoveDestination(FVector DestLocation)
{
	GameInstance->GetPlayerMoveController()->SetDestination(DestLocation);
	if (GameInstance->GetCombatManager()->CheckCombatState(PlayerTurn) &&
		GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickSkill))
	{
		APlayerCharacter* ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
		AController* Controller = ControlCharacter->GetController();
		if (Controller)
		{
			ACharacterAIController* AiController = Cast<ACharacterAIController>(Controller);
			if (AiController)
			{
				if (ControlCharacter->GetCurrentCost() - requireCost >= 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Use Move Cost"));
					GameInstance->GetPlayerCombatController()->SetPlayerInputState(Moving, requireCost);
					ControlCharacter->AddMoveCost(requireCost);
					ControlCharacter->UseMoveCost(MoveDistance);

					AiController->CombatMoveToDestination();

					OnSetDestinationReleased();
				}
				else
				{
					/*FText annotation_text = FText(NSLOCTEXT("InGameUIAnnotationText", "Arakadia", "이동하기 위한 코스트가 부족합니다!"));
					GameInstance->InGameUI->AddAnnotation(annotation_text);*/
				}
			}
		}
	}
	else
	{
		GameInstance->GetPlayerMoveController()->SetDestination(DestLocation);
	}
}

void AGPlayerController::OnSetDestinationPressed()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}
	ALootingActor* target = Cast<ALootingActor>(_cursorCharacterTarget_Temp);
	ANonPlayerCharacter* nontarget = Cast<ANonPlayerCharacter>(_cursorCharacterTarget_Temp);

	if (GameInstance->GetInputState() == EInputState::State::Default)
	{
		if (target)
		{
			if (!target->isLootingEmpty())
			{
				APlayerCharacter* ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
				if (Distance_LootingTarget > ControlCharacter->GetDistanceTo(target))
				{
					GetMousePosition(LootingUIPosition.X, LootingUIPosition.Y);
					target->OpenLootingUI(LootingUIPosition);
					LootingUIOn = true;

					return;
				}
				else
				{
					SetNewMoveDestination(target->GetActorLocation());
					IsGoingLootingTarget = true;
					GoingLootingTarget = target;
					//GameInstance->GetPlayerMoveController()->SetDestination(DestLocation);
				}
			}
		}
		//else
		//{
		//	GameInstance->GetGameMenuManager()->OffLootingUI();
		//}
	}

	if (nontarget)
	{
		if (!nontarget->isLootingEmpty())
		{
			APlayerCharacter* ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
			if (nontarget->GetBDeath())
			{
				if (ControlCharacter && (Distance_LootingTarget > ControlCharacter->GetDistanceTo(nontarget)))
				{
					GetMousePosition(LootingUIPosition.X, LootingUIPosition.Y);
					nontarget->OpenLootingUI(LootingUIPosition);
					LootingUIOn = true;
					return;
				}
				if ((_combatManager->CheckCombatTure() && (ControlCharacter->GetCurrentCost() - requireCost >= 0)) ||
					!_combatManager->CheckCombatTure())
				{
					SetNewMoveDestination(nontarget->GetActorLocation());
					IsGoingLootingTarget = true;
					GoingLootingTarget = nontarget;
					//GameInstance->GetPlayerMoveController()->SetDestination(DestLocation);
				}
			}
		}
	}

	FVector2D mousepos;
	GetMousePosition(mousepos.X, mousepos.Y);
	if (!IsLootingUIClicked(mousepos) && LootingUIOn)
	{
		GameInstance->GetGameMenuManager()->OffLootingUI();
		LootingUIOn = false;
		LootingUIPosition = FVector2D(-1, -1);
	}
	else if (IsLootingUIClicked(mousepos) && LootingUIOn)
	{
		IsMouseOnLootingUI = false;
		return;
	}

	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AGPlayerController::OnSetDestinationReleased()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AGPlayerController::OnCharacterChange(int8 order)
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog || GameInstance->GetInputState() ==
		EInputState::State::Combat)
	{
		return;
	}
	//AGGameMode* GameMode = Cast<AGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	int characterNum = order - 1;
	GameInstance->GetInGameDataManager()->SetControlCharacter(static_cast<ECharacterName>(characterNum));
	GameInstance->GetInGameUI()->UpdateSkillPortrait();
}

void AGPlayerController::OnClickUseSkill()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}

	if (_combatManager->CheckCombatState(PlayerTurn) &&
		GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickTarget))
	{
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(BeforeActionSkill);
	}
	else if (!_combatManager->CheckCombatTure() && _cursorCharacterTarget)
	{
		//_cursorCharacterTarget->Respawn();
	}

	AGPlayerCamera* camera = Cast<AGPlayerCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (camera)
	{
		camera->StopRotateCamera();
	}
}

void AGPlayerController::OnLMBPressed()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}

	AGPlayerCamera* camera = Cast<AGPlayerCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (camera)
	{
		camera->StartRotateCamera();
	}

	FVector2D mousepos;
	GetMousePosition(mousepos.X, mousepos.Y);

	if (!IsLootingUIClicked(mousepos) && LootingUIOn)
	{
		GameInstance->GetGameMenuManager()->OffLootingUI();
		LootingUIOn = false;
		LootingUIPosition = FVector2D(-1, -1);
	}
	else if (IsLootingUIClicked(mousepos) && LootingUIOn)
	{
		IsMouseOnLootingUI = false;
	}
}

void AGPlayerController::OnClickCancelAction()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}
	if (GameInstance->GetCombatManager()->CheckCombatTure())
	{
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickSkill);
	}
}

void AGPlayerController::DrawPath_Implementation()
{
}

void AGPlayerController::SetDistanceWidgetVisibility(bool bValue)
{
	if (DistanceTextWidget && bValue)
	{
		DistanceTextWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DistanceTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

float AGPlayerController::ChangeDistanceNotation(float value)
{
	return (value / 100.0f);
}

float AGPlayerController::CalDistance(FVector& Start, FVector& End)
{
	float dis = FVector::Dist(Start, End);
	return dis / 100;
}

bool AGPlayerController::GetMousePointNavDistance(float& _out)
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		FVector PlayerLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();

		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSys)
		{
			UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(
				GetWorld(), PlayerLocation, Hit.ImpactPoint);
			if (NavPath != nullptr && NavPath->IsValid())
			{
				_out = NavPath->GetPathLength();
				return true;
			}
		}
	}

	return false;
}

void AGPlayerController::UpdateDistanceText(float distance)
{
	if (DistanceTextWidget)
	{
		UTextBlock* DistanceTextBlock = Cast<UTextBlock>(
			DistanceTextWidget->GetWidgetFromName(TEXT("DistanceTextBlock")));
		if (DistanceTextBlock)
		{
			APlayerCharacter* playerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();

			float moveCost = playerCharacter->GetMoveCost();
			int movePerCost = playerCharacter->GetCharacterInfoManager()->GetMovePerCost();
			float moveCostDisanceMeter = ChangeDistanceNotation(moveCost);
			float moveDistanceMeter = ChangeDistanceNotation(distance);

			FString DistanceString = FString::Printf(TEXT("%.2fm / %.2fm"), moveDistanceMeter, moveCostDisanceMeter);
			if (moveCost > distance)
			{
				DistanceTextBlock->SetColorAndOpacity(FLinearColor::White);
				requireCost = 0;
			}
			else
			{
				int playerCurCost = playerCharacter->GetCurrentCost();
				requireCost = CalRequirementCost(moveDistanceMeter - moveCostDisanceMeter, movePerCost);
				if (playerCurCost >= requireCost)
				{
					DistanceTextBlock->SetColorAndOpacity(FLinearColor::Blue);
					DistanceString = FString::Printf(TEXT("%d 행동력"), requireCost);
				}
				else
				{
					DistanceTextBlock->SetColorAndOpacity(FLinearColor::Red);
					DistanceString = FString::Printf(TEXT("%d 행동력 부족"), requireCost);
				}
			}
			DistanceTextBlock->SetText(FText::FromString(DistanceString));

			FVector2D MouseScreenPosition;
			GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);

			FVector2D WidgetPosition = FVector2D(MouseScreenPosition.X + DistanceCursorPosX,
			                                     MouseScreenPosition.Y + DistanceCursorPosY);

			DistanceTextWidget->SetPositionInViewport(WidgetPosition);
		}
	}
}

//Interact
void AGPlayerController::OnInteract()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog || GameInstance->GetInputState() ==
		EInputState::State::Combat)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter;
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	PlayerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	if (PlayerCharacter != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact_Getcharacter success"));
		GameInstance->SetInputState(EInputState::State::Dialog);
		PlayerCharacter->Interact_Pressed();
	}
	else
	{
	}
}

void AGPlayerController::OnDialogAccept()
{
	//UE_LOG(LogTemp, Warning, TEXT("GPlayerController(637) OnDialogAccept execute"));
	if (GameInstance->GetInputState() != EInputState::State::Dialog)
	{
		return;
	}

	UDialogComponentBase* DialogComponent = nullptr;
	APlayerCharacter* PlayerCharacter;
	PlayerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	if (PlayerCharacter != nullptr)
	{
		if (PlayerCharacter->GetInteractableinRange().Num() == 0)
			return;

		AInteractionCharacter* interchar = Cast<AInteractionCharacter>(PlayerCharacter->GetInteractableinRange()[0]);

		if (interchar)
		{
			DialogComponent = interchar->GetDialogComponent();
		}
		else
		{
			DialogComponent = Cast<UDialogComponentBase>(PlayerCharacter->GetInteractableinRange()[0]);
		}
		if (DialogComponent)
			DialogComponent->GetDialogWidget()->Accept();
	}
}

void AGPlayerController::OnDialogChooseUp()
{
	//UE_LOG(LogTemp, Warning, TEXT("GPlayerController(663) OnDialogChooseUp execute"));
	if (GameInstance->GetInputState() != EInputState::State::Dialog)
	{
		return;
	}

	UDialogComponentBase* DialogComponent = nullptr;
	APlayerCharacter* PlayerCharacter;
	PlayerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	if (PlayerCharacter != nullptr)
	{
		if (PlayerCharacter->GetInteractableinRange().Num() == 0)
			return;

		AInteractionCharacter* interchar = Cast<AInteractionCharacter>(PlayerCharacter->GetInteractableinRange()[0]);
		if (interchar)
		{
			DialogComponent = interchar->GetDialogComponent();
		}
		else
		{
			DialogComponent = Cast<UDialogComponentBase>(PlayerCharacter->GetInteractableinRange()[0]);
		}
		if (DialogComponent)
			DialogComponent->GetDialogWidget()->ChooseUp();
	}
}

void AGPlayerController::OnDialogChooseDown()
{
	//UE_LOG(LogTemp, Warning, TEXT("GPlayerController(689) OnDialogChooseDown execute"));
	if (GameInstance->GetInputState() != EInputState::State::Dialog)
	{
		return;
	}

	UDialogComponentBase* DialogComponent = nullptr;
	APlayerCharacter* PlayerCharacter;

	PlayerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	if (PlayerCharacter != nullptr)
	{
		if (PlayerCharacter->GetInteractableinRange().Num() == 0)
			return;

		AInteractionCharacter* interchar = Cast<AInteractionCharacter>(PlayerCharacter->GetInteractableinRange()[0]);
		if (interchar)
		{
			DialogComponent = interchar->GetDialogComponent();
		}
		else
		{
			DialogComponent = Cast<UDialogComponentBase>(PlayerCharacter->GetInteractableinRange()[0]);
		}
		if (DialogComponent)
			DialogComponent->GetDialogWidget()->ChooseDown();
	}
}

void AGPlayerController::OnDialogAcceptMouse()
{
	//UE_LOG(LogTemp, Warning, TEXT("GPlayerController(716) OnDialogAcceptMouse execute"));
	if (GameInstance->GetInputState() != EInputState::State::Dialog)
	{
		return;
	}

	UDialogComponentBase* DialogComponent = nullptr;
	APlayerCharacter* PlayerCharacter;
	PlayerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	if (PlayerCharacter != nullptr)
	{
		if (PlayerCharacter->GetInteractableinRange().Num() == 0)
			return;

		AInteractionCharacter* interchar = Cast<AInteractionCharacter>(PlayerCharacter->GetInteractableinRange()[0]);
		if (interchar)
		{
			DialogComponent = interchar->GetDialogComponent();
		}
		else
		{
			DialogComponent = Cast<UDialogComponentBase>(PlayerCharacter->GetInteractableinRange()[0]);
		}
		if(DialogComponent)
			DialogComponent->GetDialogWidget()->AcceptMouse();
	}
}


void AGPlayerController::SetDoubleGameSpeed()
{
	if (GameInstance->GetInputState() == EInputState::State::Combat)
	{
		GetWorldSettings()->SetTimeDilation(2.0f);
	}
}

void AGPlayerController::SetDefaultGameSpeed()
{
	GetWorldSettings()->SetTimeDilation(1.0f);
}

void AGPlayerController::OnESCMenu()
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		UDialogComponentBase* DialogComponent = nullptr;
		APlayerCharacter* PlayerCharacter;
		PlayerCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
		if (PlayerCharacter != nullptr)
		{
			if (PlayerCharacter->GetInteractableinRange().Num() == 0)
				return;

			AInteractionCharacter* interchar = Cast<AInteractionCharacter>(PlayerCharacter->GetInteractableinRange()[0]);
			if (interchar)
			{
				DialogComponent = interchar->GetDialogComponent();
			}
			else
			{
				DialogComponent = Cast<UDialogComponentBase>(PlayerCharacter->GetInteractableinRange()[0]);
			}
			if (DialogComponent)
				DialogComponent->GetDialogWidget()->Cancel();
			return;
		}
	}

	UGGameInstance* game_instance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	game_instance->GetInGameUI()->GetEscMenu()->On();
}

void AGPlayerController::ChangeCursor(TEnumAsByte<EMouseCursor::Type> cursorType)
{
	CurrentMouseCursor = cursorType;
	switch (cursorType)
	{
	case EMouseCursor::Default:
		break;
	default:
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
		break;
	}
}

void AGPlayerController::ChangeSelectedCursorTarget(AGCharacter* target)
{
	if (!_cursorCharacterTarget && target)
	{
		_cursorCharacterTarget = target;
		_cursorCharacterTarget->ActiveHighlight();
	}
	else if (target && _cursorCharacterTarget != target)
	{
		_cursorCharacterTarget->DeActiveHighlight();
		_cursorCharacterTarget = target;
		_cursorCharacterTarget->ActiveHighlight();
	}
	else if (target && _cursorCharacterTarget == target)
	{
		_cursorCharacterTarget->ActiveHighlight();
	}
	else
	{
		if (_cursorCharacterTarget)
		{
			_cursorCharacterTarget->DeActiveHighlight();
		}
		_cursorCharacterTarget = nullptr;
	}
}

void AGPlayerController::ChangeSelectedCursorTarget_Temp(AActor* target)
{
	//CurrentMouseCursor = EMouseCursor::CardinalCross;
	//CurrentMouseCursor = EMouseCursor::Default;

	if (!_cursorCharacterTarget_Temp && target)
	{
		//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller 827 - target name %s"), *(target->GetName()));
		_cursorCharacterTarget_Temp = target;
		ActiveHighlight(_cursorCharacterTarget_Temp);
	}
	else if (target && _cursorCharacterTarget_Temp != target)
	{
		//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller 833 - target name %s"), *(target->GetName()));
		DeActiveHighlight(_cursorCharacterTarget_Temp);
		_cursorCharacterTarget_Temp = target;
		ActiveHighlight(_cursorCharacterTarget_Temp);
	}
	else if (target && _cursorCharacterTarget_Temp == target)
	{
		//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller 839 - target name %s"), *(target->GetName()));
		ActiveHighlight(_cursorCharacterTarget_Temp);
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller 845 - target nullptr "));
		if (_cursorCharacterTarget_Temp)
		{
			DeActiveHighlight(_cursorCharacterTarget_Temp);
		}
		_cursorCharacterTarget_Temp = nullptr;
		ChangeCursor(EMouseCursor::Default);
	}
}

void AGPlayerController::ActiveHighlight(AActor* target)
{
	//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller ActiveHighlight 853 - excute "));
	FVector2D MouseScreenPosition;
	GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);

	if (target == nullptr)
	{
		ChangeCursor(EMouseCursor::Default);
	}

	APlayerCharacter* ControlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
	AInteractionCharacter* InteractCharacter = Cast<AInteractionCharacter>(target);
	AGCharacter* TargetCharacter = Cast<AGCharacter>(target);
	ALootingActor* LootingActor = Cast<ALootingActor>(target);
	if (InteractCharacter)
	{
		if (InteractCharacter->GetQuestVisible()
			&& ((GameInstance->GetInputState() != EInputState::State::Dialog) && (GameInstance->GetInputState() != EInputState::State::UI))
			&& (ControlCharacter->GetDistanceTo(InteractCharacter) < 2250.0f))
		{
			FVector2D interactpos;
			this->ProjectWorldLocationToScreen(InteractCharacter->GetActorLocation(), interactpos);
			InteractCharacter->ActiveHighlight(MouseScreenPosition);
			GameInstance->GetInGameUI()->OnInteractCharInfoUI(InteractCharacter->CharName, interactpos, InteractCharacter->InfoOffset);
		}
	}
	else if (LootingActor)
	{
		if (!LootingActor->isLootingEmpty() && (ControlCharacter->GetDistanceTo(LootingActor) < 2250.0f))
		{
			FVector2D lootingpos;
			this->ProjectWorldLocationToScreen(LootingActor->GetActorLocation(), lootingpos);
			LootingActor->ActiveHighlight(MouseScreenPosition);
			GameInstance->GetInGameUI()->OnInteractCharInfoUI(LootingActor->CharName, lootingpos, LootingActor->InfoOffset);
		}
	}
	else if (TargetCharacter && !TargetCharacter->GetBDeath() && _combatManager->CheckCombatTure())
	{
		//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller ActiveHighlight 871 - combattrue highlight %s"), *(TargetCharacter->GetName()));
		TargetCharacter->ActiveHighlight();
	}
	else if (TargetCharacter && TargetCharacter->GetBDeath())
	{
		ANonPlayerCharacter* nontarget = Cast<ANonPlayerCharacter>(TargetCharacter);

		if (nontarget)
		{
			if (nontarget->isLootingEmpty())
				return;
		}
		ChangeCursor(EMouseCursor::CardinalCross);
		TargetCharacter->ActiveHighlight();
	}
}

void AGPlayerController::DeActiveHighlight(AActor* target)
{
	//UE_LOG(LogTemp, Log, TEXT("Gplayercontroller DeActiveHighlight 887 - execute"));
	if (target == nullptr)
	{
		ChangeCursor(EMouseCursor::Default);
	}

	AInteractionCharacter* InteractCharacter = Cast<AInteractionCharacter>(target);
	AGCharacter* TargetCharacter = Cast<AGCharacter>(target);
	ALootingActor* LootingActor = Cast<ALootingActor>(target);
	if (InteractCharacter)
	{
		GameInstance->GetInGameUI()->OffInteractCharInfoUI();
		InteractCharacter->DeActiveHighlight();
	}
	else if (LootingActor)
	{
		GameInstance->GetInGameUI()->OffInteractCharInfoUI();
		LootingActor->DeActiveHighlight();
	}
	else if (TargetCharacter && !TargetCharacter->GetBDeath() && _combatManager->CheckCombatTure())
	{
		TargetCharacter->DeActiveHighlight();
	}
	else if (TargetCharacter && TargetCharacter->GetBDeath())
	{
		TargetCharacter->DeActiveHighlight();
	}
}


void AGPlayerController::ControlGameMenuUI(UI_TYPE ui)
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}

	UGameMenuManager* GameMenuManager = GameInstance->GetGameMenuManager();

	GameMenuManager->On(ui);
	GameMenuManager->SetKeyboardFocus();

	//GameInstance->GameMenuUI->SetFocus();
	SetInputMode(FInputModeUIOnly());
}

bool AGPlayerController::IsLootingUIClicked(FVector2D pos)
{
	return IsMouseOnLootingUI;
}
