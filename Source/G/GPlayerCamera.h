// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GPlayerCamera.generated.h"

/*
* 
* 플레이어가 보는 카메라
* W,A,S,D로 이동하고 Spacebar 입력 시 현재 컨트톨 캐릭터 위로 이동
* DefaultPawn으로 설정, 위치는 에디터에 배치된 NetworkPlayerStart에서 생성
* 
*/

class CameraMovementInfo
{
public:
	float CurrentSpeed = 0.0f;
	float TargetSpeed = 0.0f;
	float AmountOfAcceleration = 600.0f;
	bool isBigger = false;
	bool CharMove = false;

	FVector FreeMoveInputInfo = FVector(0.0f, 0.0f, 0.0f);
	FVector FreeMoveDirection = FVector(0.0f, 0.0f, 0.0f);

	CameraMovementInfo()
	{
	}

	~CameraMovementInfo()
	{
	}
};

UCLASS()
class G_API AGPlayerCamera : public APawn
{
	GENERATED_BODY()

	UPROPERTY()
	class APlayerController* player_controller;

	UPROPERTY()
	class AGGameMode* GameMode;

	UPROPERTY()
	class UGGameInstance* GameInstance;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	bool bRotateCamera; // camera rotate ON / OFF 여부
	
	FTimerHandle rotateControlCharacterTimer;
	// bool bRotateCameraYaw;
	// bool bRotateCameraPitch;

	FVector2D InitialMousePosition;

	float YawRotationSpeed;
	float PitchRotationSpeed;

	FRotator max_rotation;
	FRotator min_rotation;

	float minTargetArmLength;
	float maxTargetArmLength;
	float TargetPitch = -45.0f;
	
	float minCameraPitch = -65.0f;
	float maxCameraPitch = 0.0f;

protected:	
	float CameraSpeed = 1.0f;

	float MaxZ = 3000.0f;
	float MinZ = 500.0f;
	float MaxX = 1500.0f;
	float MinX = -1500.0f;
	float MaxY = 1500.0f;
	float MinY = -1500.0f;

	bool ControlCamera = false;

	CameraMovementInfo CameraMoveInfo = CameraMovementInfo();

	int32 PreviousZoomLevel = -1;
	
	// 카메라 줌 단계별 길이 설정 (ZoomLevels)
	float ZoomLevels[10] = { 500.0f, 700.0f, 900.0f, 1100.0f, 1300.0f, 1500.0f, 1700.0f, 1800.0f, 1900.0f, 2000.0f };

	// 카메라와 캐릭터 간의 거리 임계값 설정 (DistanceThresholds)
	float DistanceThresholds[10] = { 600.0f, 800.0f, 1000.0f, 1200.0f, 1400.0f, 1600.0f, 1800.0f, 1850.0f, 1950.0f, 2000.0f };

	// 현재 줌 레벨 (0부터 시작하여 최대 9까지)
	int32 CurrentZoomLevel = 5;  // 기본으로 중간 단계에서 시작

	// 카메라 부드러운 전환 속도
	float ZoomInterpSpeed = 5.0f;

	UPROPERTY()
	class AGCharacter* lastControlCharacter = nullptr;

	bool EntryBattle = false;

private :
	void RotateCamera(float DeltaTime);
	void CameraMoveTick(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 줌 인/아웃 함수 (거리에 따라 자동으로 조정)
	void AdjustZoomByDistance(float DeltaTime);

	// 부드럽게 줌을 전환하는 함수
	void AdjustCameraZoomAndPitchOnChange(float DeltaTime);
	
	// 카메라 이동
	UFUNCTION()
	void MoveForward(float AxisValue);
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void MoveZoomInOut(float AxisValue);
	UFUNCTION()
	void MoveControlCharacter();

public:
	// Sets default values for this character's properties
	AGPlayerCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartRotateCamera();
	void StopRotateCamera();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void SetCameraMovSpeed(float speed);
	void SetCameraRotSpeed(float speed);

	bool RotateControlCharacterCamera(AGCharacter* character);
};
