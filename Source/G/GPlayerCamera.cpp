// Fill out your copyright notice in the Description page of Project Settings.


#include "GPlayerCamera.h"
#include "G/GGameMode.h"
#include "G/GGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>

#include "G.h"
#include "G/GPlayerController.h"
#include "Manager/CombatManager.h"
#include "G/NonPlayerCharacter.h"
#include "G/PlayerCharacter.h"

// Called when the game starts or when spawned
void AGPlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FVector MoveLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
	SetActorLocation(FVector(MoveLocation.X, MoveLocation.Y, MoveLocation.Z));

	CameraBoom->SetRelativeRotation(FRotator(-40.0f, 0.f, 0.f));

	TopDownCameraComponent->AddLocalRotation(FRotator(0, 0, 0).Quaternion());
	
	CameraBoom->TargetOffset = FVector(0, 0, 0);
	CameraBoom->TargetArmLength = 1000.0f;

	AGPlayerController* PlayerController = Cast<AGPlayerController>(GetController());
	player_controller = PlayerController;
}

// Sets default values
AGPlayerCamera::AGPlayerCamera()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SetRootComponent(CameraBoom);

	//CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.0f;
	// 카메라 각도 조절
	CameraBoom->SetRelativeRotation(FRotator(-40.0f, 0.f, 0.f));

	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bRotateCamera = false;

	max_rotation.Pitch = -5.0f;
	min_rotation.Pitch = -85.0f;

	YawRotationSpeed = 10.0f;
	PitchRotationSpeed = 10.0f;

	CameraBoom->TargetOffset = FVector(0, 0, 0);

	minTargetArmLength = 500.0f;
	maxTargetArmLength = 2000.0f;
}


void AGPlayerCamera::RotateCamera(float DeltaTime)
{
	//if (bRotateCamera)
	if (bRotateCamera)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			// 현재 마우스 위치 얻기
			FVector2D CurrentMousePosition;
			PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);

			// 마우스 이동량 계산
			FVector2D MouseDelta = CurrentMousePosition - InitialMousePosition;

			// 카메라 회전을 적용
			FRotator NewRotation = CameraBoom->GetComponentRotation();
			if (bRotateCamera)
			{
				PreviousZoomLevel = CurrentZoomLevel;
				NewRotation.Yaw += YawRotationSpeed * DeltaTime * MouseDelta.X;
			}
			if (bRotateCamera)
			{
				// 마우스가 화면 밖으로 나갔을 때, 예외 처리 
				if (MouseDelta.Y > -100 && MouseDelta.Y < 100)
				{
					float final_pitch_rotation_speed = PitchRotationSpeed * DeltaTime * MouseDelta.Y;
					if (MouseDelta.Y < 0 && min_rotation.Pitch < NewRotation.Pitch)
					{
						if (min_rotation.Pitch >= NewRotation.Pitch + final_pitch_rotation_speed)
						{
							NewRotation.Pitch = min_rotation.Pitch;
						}
						else
						{
							NewRotation.Pitch += final_pitch_rotation_speed;
						}
					}
					else if (MouseDelta.Y > 0 && max_rotation.Pitch > NewRotation.Pitch + final_pitch_rotation_speed)
					{
						if (max_rotation.Pitch <= NewRotation.Pitch + final_pitch_rotation_speed)
						{
							NewRotation.Pitch = max_rotation.Pitch;
						}
						else
						{
							NewRotation.Pitch += final_pitch_rotation_speed;
						}
					}
				}
			}

			CameraBoom->SetWorldRotation(NewRotation.Quaternion());

			// 다음 프레임을 위해 초기 마우스 위치 갱신
			InitialMousePosition = CurrentMousePosition;
		}
	}
}

bool AGPlayerCamera::RotateControlCharacterCamera(AGCharacter* character)
{
	LOG(Log, "Rotate!");
	const FRotator TargetRotation = character->GetActorRotation();
	const FRotator CurrentRoation = GetActorRotation();
	FRotator NewCameraRotation = FMath::RInterpTo(CurrentRoation, TargetRotation, GetWorld()->GetDeltaSeconds(), YawRotationSpeed);  // 부드러운 이동 처리
	NewCameraRotation.Roll = CurrentRoation.Roll;
	NewCameraRotation.Pitch = CurrentRoation.Pitch;
	SetActorRotation(NewCameraRotation);

	if(FMath::IsNearlyEqual(NewCameraRotation.Yaw, TargetRotation.Yaw, 1.0f))
	{
		LOG(Log, "%s %s, Rotate FIN!", *NewCameraRotation.ToString(), *TargetRotation.ToString());
		return true;
	}
	return false;
}

// Called every frame
void AGPlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 컨트롤하지 않을 때, 플레이어 캐릭터에 고정
	
	CameraMoveTick(DeltaTime);
	
	RotateCamera(DeltaTime);
	// 거리 기반 줌 조정
	AdjustZoomByDistance(DeltaTime);
}

// Called to bind functionality to input
void AGPlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 카메라 컨트롤
	PlayerInputComponent->BindAxis("MoveForward", this, &AGPlayerCamera::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGPlayerCamera::MoveRight);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AGPlayerCamera::MoveZoomInOut);

	// 스페이스 입력
	PlayerInputComponent->BindAction("SpaceButton", IE_Pressed, this, &AGPlayerCamera::MoveControlCharacter);
}

void AGPlayerCamera::StartRotateCamera()
{
	bRotateCamera = true;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->GetMousePosition(InitialMousePosition.X, InitialMousePosition.Y);
	}
}

void AGPlayerCamera::StopRotateCamera()
{
	bRotateCamera = false;
}

void AGPlayerCamera::SetCameraMovSpeed(float speed)
{
	CameraSpeed = speed;
}

void AGPlayerCamera::SetCameraRotSpeed(float speed)
{
	YawRotationSpeed = speed;
	PitchRotationSpeed = speed;
}

void AGPlayerCamera::MoveControlCharacter()
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->CallOnDialogAccept();
	}

	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}
	//FVector MoveLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
	//SetActorLocation(MoveLocation);
	ControlCamera = !ControlCamera;
}

void AGPlayerCamera::MoveZoomInOut(float AxisValue)
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}

	if (CameraBoom->TargetArmLength >= minTargetArmLength && CameraBoom->TargetArmLength <= maxTargetArmLength && AxisValue != 0)
	{
		CameraBoom->TargetArmLength += AxisValue * 150;
		// 카메라의 현재 회전 값
		FRotator CurrentRotation = CameraBoom->GetComponentRotation();
		// 줌 단계에 따른 목표 피치 값 설정
		float targetPitch = CurrentRotation.Pitch + (-AxisValue * 5.0f);
		if(targetPitch < maxCameraPitch && targetPitch > minCameraPitch)
		{
			TargetPitch = targetPitch;
		}
		
	}
	else if (CameraBoom->TargetArmLength < minTargetArmLength && AxisValue > 0)
	{
		CameraBoom->TargetArmLength += AxisValue * 150;
	}
	else if (CameraBoom->TargetArmLength > maxTargetArmLength && AxisValue < 0)
	{
		CameraBoom->TargetArmLength += AxisValue * 150;
	}
	/*
	else if (*cur_controller_type == controller_type::UI) {
		if (GameInstance->get_current_ui() == UI_TYPE::Map) {
			AWorldSceneCapture2D* world_map_camera = GameInstance->get_world_map_camera();

			FVector direction = FVector(0, 0, 1.0f) * AxisValue * world_map_cam_speed;

			world_map_camera->AddActorWorldOffset(direction);
		}
	}
	*/
}

void AGPlayerCamera::CameraMoveTick(float DeltaTime)
{
	if(!GameInstance->GetCombatManager()->CheckCombatTure())
		EntryBattle = false;

    // 카메라를 자유 이동 중
    if (ControlCamera)
    {
        // 카메라 회전 대응 : 회전된 카메라를 기준으로 X축과 Y축의 벡터를 가져옴
        FVector XDirection = FRotationMatrix(CameraBoom->GetComponentRotation()).GetScaledAxis(EAxis::X);
        XDirection.Normalize();
        XDirection.Z = 0;

        FVector YDirection = FRotationMatrix(CameraBoom->GetComponentRotation()).GetScaledAxis(EAxis::Y);
        YDirection.Normalize();
        YDirection.Z = 0;

        // 입력에 따라 카메라 이동 방향 설정
        FVector Direction = CameraMoveInfo.FreeMoveInputInfo;
        Direction.Normalize();

        // 가속도 설정
        float TargetSpeed = (Direction.Size() > 0) ? 1200.0f * CameraSpeed : 0.0f;
        CameraMoveInfo.CurrentSpeed = FMath::FInterpTo(CameraMoveInfo.CurrentSpeed, TargetSpeed, DeltaTime, 5.0f);  // 부드러운 속도 변화

        // 최종 이동 벡터 계산
        FVector MoveValue = Direction * CameraMoveInfo.CurrentSpeed * DeltaTime;
        XDirection *= MoveValue.X;
        YDirection *= MoveValue.Y;

        FVector FinalMoveValue = XDirection + YDirection;

        // 캐릭터의 위치를 기준으로 이동 범위를 제한
        FVector CharacterLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
        FVector NewCameraLocation = GetActorLocation() + FinalMoveValue;

        // X축, Y축 이동 범위를 Clamp로 제한
        NewCameraLocation.X = FMath::Clamp(NewCameraLocation.X, CharacterLocation.X + MinX, CharacterLocation.X + MaxX);
        NewCameraLocation.Y = FMath::Clamp(NewCameraLocation.Y, CharacterLocation.Y + MinY, CharacterLocation.Y + MaxY);

        // 카메라 위치 적용
        SetActorLocation(NewCameraLocation);
    }
    // 카메라가 캐릭터를 따라갈 때
    else
    {
        AGCharacter* controlCharacter = nullptr;
        // 전투 중일 때
        if (GameInstance->GetCombatManager()->CheckCombatTure())
        {
			controlCharacter = GameInstance->GetCombatManager()->GetCurTrunCharacter();
			if (!EntryBattle)
			{
				APlayerCharacter* playchar = Cast< APlayerCharacter>(controlCharacter);
				ANonPlayerCharacter* nonchar = Cast<ANonPlayerCharacter>(controlCharacter);

				if (playchar)
				{
					EntryBattle = true;
				}
				else if (nonchar)
				{
					if (lastControlCharacter == controlCharacter)
					{
						controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
					}
					else
					{
						EntryBattle = true;
						lastControlCharacter = controlCharacter;
					}
				}
			}
			else
			{
				lastControlCharacter = controlCharacter;
			}
        }
        // 전투 중이 아닐 때
        else
        {
            controlCharacter = GameInstance->GetInGameDataManager()->GetControlCharacter();
        }

        if (!controlCharacter)
        {
            return;
        }

        // 캐릭터의 위치로 카메라를 부드럽게 이동
        FVector TargetLocation = controlCharacter->GetActorLocation();
        FVector CurrentLocation = GetActorLocation();
        FVector NewCameraLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f);  // 부드러운 이동 처리
    	
        // 이동한 카메라 위치 적용
        SetActorLocation(NewCameraLocation);
    	
    }
}


void AGPlayerCamera::MoveForward(float AxisValue)
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}

	CameraMoveInfo.FreeMoveInputInfo.X = AxisValue;

	//FVector CharacterLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
	//FVector CameraLocation = GetActorLocation();

	//float MoveValue = AxisValue * CameraSpeed;
	//float MoveX = CameraLocation.X + MoveValue;

	//// 카메라 회전에 따른 이동에 추가된 코드
	//FVector Direction = FRotationMatrix(CameraBoom->GetComponentRotation()).GetScaledAxis(EAxis::X);
	//Direction.Normalize();
	//Direction *= MoveValue;
	//if (CharacterLocation.X + MinX <= MoveX && MoveValue <= 0)
	//{
	//	Direction.Z = 0;
	//	AddActorWorldOffset(Direction);
	//	// 카메라 회전에 따른 이동을 구현하기 위해 기존 함수에서 아래 함수로 변경, 움직이지 않음.
	//	//AddMovementInput(FVector(1,0,0), CameraSpeed * 100);
	//}
	//else if (MoveX <= CharacterLocation.X + MaxX && MoveValue > 0)
	//{
	//	Direction.Z = 0;
	//	AddActorWorldOffset(Direction);
	//	//AddActorWorldOffset(FVector(MoveValue, 0.0f, 0.0f));
	//	//AddMovementInput(FVector(1, 0, 0), CameraSpeed*100);
	//}

	/*
	else if (*cur_controller_type == controller_type::UI) {
		if (GameInstance->get_current_ui() == UI_TYPE::Map) {
			AWorldSceneCapture2D* world_map_camera = GameInstance->get_world_map_camera();

			FVector direction = FVector(1.0f, 0, 0) * AxisValue * world_map_cam_speed;

			world_map_camera->AddActorWorldOffset(direction);
		}
	}
	*/
}

void AGPlayerCamera::MoveRight(float AxisValue)
{
	if (GameInstance->GetInputState() == EInputState::State::Dialog)
	{
		return;
	}

	CameraMoveInfo.FreeMoveInputInfo.Y = AxisValue;

	//FVector CharacterLocation = GameInstance->GetInGameDataManager()->GetControlCharacterLoaction();
	//FVector CameraLocation = GetActorLocation();
	//float MoveValue = AxisValue * CameraSpeed;
	//float MoveY = CameraLocation.Y + MoveValue;

	//FVector Direction = FRotationMatrix(CameraBoom->GetComponentRotation()).GetScaledAxis(EAxis::Y);
	//Direction.Normalize();

	//Direction *= MoveValue;

	//if (CharacterLocation.Y + MinY <= MoveY && MoveValue <= 0)
	//{
	//	Direction.Z = 0;
	//	AddActorWorldOffset(Direction);
	//	//AddActorWorldOffset(FVector(0.0f, MoveValue, 0.0f));
	//}
	//else if (MoveY <= CharacterLocation.Y + MaxY && MoveValue > 0)
	//{
	//	Direction.Z = 0;
	//	AddActorWorldOffset(Direction);
	//	//AddActorWorldOffset(FVector(0.0f, MoveValue, 0.0f));
	//}
	/*
	else if (*cur_controller_type == controller_type::UI) {
		if (GameInstance->get_current_ui() == UI_TYPE::Map) {
			AWorldSceneCapture2D* world_map_camera = GameInstance->get_world_map_camera();

			FVector direction = FVector(0, 1.0f, 0) * AxisValue * world_map_cam_speed;

			world_map_camera->AddActorWorldOffset(direction);
		}
	}
	*/
}

void AGPlayerCamera::AdjustZoomByDistance(float DeltaTime)
{
	// 현재 컨트롤 캐릭터의 위치를 가져옴
	AGCharacter* ControlCharacter = Cast<AGCharacter>(GameInstance->GetInGameDataManager()->GetControlCharacter());
	if (!ControlCharacter)
	{
		return;
	}

	// 카메라와 캐릭터 사이의 거리 계산
	float DistanceToCharacter = CameraBoom->TargetArmLength;

	// 거리 임계값에 따라 줌 레벨 자동 조정
	for (int32 i = 0; i < 10; ++i)
	{
		if (DistanceToCharacter < DistanceThresholds[i])
		{
			CurrentZoomLevel = i;
			break;
		}
	}

	// 부드러운 줌과 회전 조정을 처리
	AdjustCameraZoomAndPitchOnChange(DeltaTime);
}

void AGPlayerCamera::AdjustCameraZoomAndPitchOnChange(float DeltaTime)
{
	// 줌 단계가 변경되었는지 확인
	if (PreviousZoomLevel != CurrentZoomLevel)
	{
		// 카메라의 현재 회전 값
		FRotator CurrentRotation = CameraBoom->GetComponentRotation();

		// 목표 피치에 도달하지 않았으면 계속 보간
		if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, TargetPitch, 0.5f))
		{
			// 목표 피치 값으로 부드럽게 보간하여 회전
			FRotator TargetRotation = FRotator(TargetPitch, CurrentRotation.Yaw, CurrentRotation.Roll);
			CameraBoom->SetWorldRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, ZoomInterpSpeed));
		}
		else
		{
			// 목표 각도에 도달했으면 피치 업데이트 중단
			PreviousZoomLevel = CurrentZoomLevel;
		}
	}

	// 줌 길이는 항상 부드럽게 보간
	float TargetZoom = ZoomLevels[CurrentZoomLevel];
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetZoom, DeltaTime, ZoomInterpSpeed);
}


