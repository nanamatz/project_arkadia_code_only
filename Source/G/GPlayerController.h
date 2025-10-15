// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GPlayerController.generated.h"

/*
* 
* 플레이어 입력 관리
* 1,2,3,4로 4개의 캐릭터 중 컨트롤 캐릭터 정할 수 있음.
* 커서 위치 출력
* 목적지 갱신
* 
*/

UCLASS(BlueprintType)
class AGPlayerController : public APlayerController
{
	GENERATED_BODY()

	/* variable */
	AGPlayerController();

	bool isGameMenuOn = false;

	UPROPERTY()
	class UGGameInstance* GameInstance;

	UPROPERTY()
	class UCombatManager* _combatManager;

	UPROPERTY()
	class AGCharacter* _cursorCharacterTarget;

	UPROPERTY()
	class AActor* _cursorCharacterTarget_Temp;
protected:
	virtual void BeginPlay() override;
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	void DefaultPlayerTick();
	void DefaultPlayerTick_Temp();
	void CombatPlayerTick();
	void OnCheat();
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	float mouseVFXtime = 0.3f;
	float mouseCurTime = 0;
	// 이동 거리
	float MoveDistance;
	// 사용 코스트
	int requireCost;

	TSubclassOf<class UUserWidget> DistanceTextWidgetClass;

	UPROPERTY()
	class UUserWidget* DistanceTextWidget;

	//TSubclassOf<class UUserWidget> InterCharInfoWidgetClass;

	//UPROPERTY()
	//class UUserWidget* InterCharInfoWidget;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	//LootingUI
	FVector2D LootingUIPosition = FVector2D(-1, -1);
	bool LootingUIOn = false;
	FVector2D LootingUISize = FVector2D(250.f, 250.f);

	bool IsGoingLootingTarget = false;
	float Distance_LootingTarget = 350.0f;

	UPROPERTY()
	class AActor* GoingLootingTarget = nullptr;

	bool IsLootingUIClicked(FVector2D pos);

public:
	bool bPlayerInputControl = true;

	bool IsMouseOnLootingUI = false;
	/* function */
private :
	void ControlGameMenuUI(enum class UI_TYPE ui);

protected:
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	// 컨트롤캐릭터 세팅 액션
	void OnCharacterChange(int8 order);

	// 좌클릭 공격 액션
	void OnClickUseSkill();
	void OnLMBPressed();

	// 액션 취소
	void OnClickCancelAction();

	// 경로 표시
	UFUNCTION(BlueprintNativeEvent)
	void DrawPath();
	// m 거리 계산
	float ChangeDistanceNotation(float value);
	// start, end m 거리 계산
	float CalDistance(FVector& Start, FVector& End);
	// nav 거리 계산
	bool GetMousePointNavDistance(float& _out);
	// 사용 Cost 계산식
	int CalRequirementCost(float distance, int movePerCost) { return distance / movePerCost + 1; }
	// 거리 텍스트 표시
	void UpdateDistanceText(float Distance);

	//상호작용
	void OnInteract();
	void OnDialogAccept();
	void OnDialogChooseUp();
	void OnDialogChooseDown();
	void OnDialogAcceptMouse();

	void SetDoubleGameSpeed();
	void SetDefaultGameSpeed();
	void OnESCMenu();

public:
	void SetInputModeGameAndUI() { SetInputMode(FInputModeGameAndUI()); };
	void SetDistanceWidgetVisibility(bool bValue);

	void ChangeCursor(TEnumAsByte<EMouseCursor::Type> cursorType);
	void ChangeSelectedCursorTarget(class AGCharacter* target);
	void ChangeSelectedCursorTarget_Temp(class AActor* target);
	void ActiveHighlight(AActor* target);
	void DeActiveHighlight(AActor* target);

	void SetInputState(EInputState::State newState) { GameInstance->SetInputState(newState); }
	void CallOnDialogAccept() { OnDialogAccept(); }

	UFUNCTION(BlueprintCallable)
	void SetInputStateDefault() { GameInstance->SetInputState(EInputState::State::Default); }
	UFUNCTION(BlueprintCallable)
		void SetMoveToMouseCursor(bool value) { bMoveToMouseCursor = value; }

	void LootingUIVariableEditOff()
	{
		LootingUIOn = false;
		LootingUIPosition = FVector2D(-1, -1);
	}
};
