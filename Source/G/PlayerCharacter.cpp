// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "CharacterAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GGameInstance.h"
#include "G/UI/InGameUI/InGameUI.h"
#include <Kismet/GameplayStatics.h>
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include <G/NonPlayerCharacter.h>
#include <G/Skill/SkillSlot.h>
#include <G/Skill/SkillBase.h>
#include "G/Dialog/InteractionInterface.h"
#include "G/GPlayerController.h"
#include "G/Dialog/InteractionCharacter.h"
#include "G/Dialog/DialogComponentBase.h"
#include "G/Dialog/DialogManageActor.h"

#include "CharacterInfoComponent.h"
#include "G.h"
#include "NavigationInvokerComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Controller/PlayerCombatController.h"
#include "Item/WeaponItemBase.h"
#include "Manager/CombatManager.h"
#include "Manager/InGameDataManager.h"
#include "Manager/InventoryManager.h"
#include "Player/SkillRangeActor.h"
#include "Skill/SkillInfoManager.h"
#include "UI/InGameUI/AnnotationWidget.h"
#include "UI/Map/PointOfInterestComponent.h"
#include "G/Manager/TutorialManager.h"

APlayerCharacter::APlayerCharacter()
{
	AIControllerClass = ACharacterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(*playerCollisionChannel);

	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComponent"));
	NavigationInvokerComponent->SetGenerationRadii(8192, 4096);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// AttackRange Decal Setting
	DecalAttackRange = CreateDefaultSubobject<UDecalComponent>(TEXT("AttackRange"));
	DecalAttackRange->SetupAttachment(GetRootComponent()); // 占실댐옙 AttachToComponent 占쏙옙占??
	static ConstructorHelpers::FObjectFinder<UMaterial> AttackRangeDecalMaterialAsset(
		TEXT("Material'/Game/CombatSystem/Materials/M_AttackRange_Decal.M_AttackRange_Decal'"));
	if (AttackRangeDecalMaterialAsset.Succeeded())
	{
		// Set the DecalComponent's material
		DecalAttackRange->SetDecalMaterial(AttackRangeDecalMaterialAsset.Object);
		DecalAttackRange->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		DecalAttackRange->SetRelativeScale3D(FVector(3, 1, 1));
		DecalAttackRange->SetHiddenInGame(true);
	}
	else
	{
	}
	// Pie Range Decal Setting
	DecalPieRange = CreateDefaultSubobject<UDecalComponent>(TEXT("PieRange"));
	DecalPieRange->SetupAttachment(GetRootComponent()); // 占실댐옙 AttachToComponent 占쏙옙占??
	static ConstructorHelpers::FObjectFinder<UMaterial> PieRangeDecalMaterialAsset(
		TEXT("Material'/Game/TopDownCPP/Blueprints/M_PielRange_Decal.M_PielRange_Decal'"));
	if (PieRangeDecalMaterialAsset.Succeeded())
	{
		// Set the DecalComponent's material
		DecalPieRange->SetDecalMaterial(PieRangeDecalMaterialAsset.Object);
		DecalPieRange->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		DecalPieRange->SetRelativeScale3D(FVector(3, 1, 1));
		DecalPieRange->SetHiddenInGame(true);
	}

	SelectedCharacterLight = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectedCharacterLight"));
	SelectedCharacterLight->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UMaterial> SelectedMaterial(
		TEXT("Material'/Game/CombatSystem/Materials/M_Combat_Select.M_Combat_Select'"));
	if (SelectedMaterial.Succeeded())
	{
		SelectedCharacterLight->SetDecalMaterial(SelectedMaterial.Object);
		SelectedCharacterLight->SetWorldLocation(FVector(0, 0, -150));
		SelectedCharacterLight->SetRelativeRotation(FRotator(90, 0, 0));
		SelectedCharacterLight->SetRelativeScale3D(FVector(1.0f, 0.3f, 0.3f));
		SelectedCharacterLight->SetHiddenInGame(true);
	}

	// InteractionSphere Set
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetSphereRadius(150.0f);
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnInteractionSphereBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnInteractionSphereEndOverlap);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	POICollision = CreateDefaultSubobject<USphereComponent>(TEXT("POICollision"));
	POICollision->SetupAttachment(RootComponent);
	POICollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	POICollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	POICollision->SetSphereRadius(5000.0f);
	POICollision->OnComponentBeginOverlap.AddDynamic(
		this, &APlayerCharacter::APlayerCharacter::OnPOIColiisionBeginOverlap);

	NavPathSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Navigation Path Spline"));
	NavPathSplineComponent->DetachFromComponent(
		FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld,
		                          true));

	GetMesh()->SetGenerateOverlapEvents(true);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SkillRangeActor = GetWorld()->SpawnActor<ASkillRangeActor>();
	SkillRangeActor->SetActorHiddenInGame(true);
	DecalAttackRange->SetHiddenInGame(true);
	_isControl = false;
	_isAbleInteract = false;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::SetIsControl(bool bValue)
{
	OffSelectCharacter();
	OffViewAllSkillRange();
	_isControl = bValue;
}

void APlayerCharacter::InitPlayerCharacter()
{
	if (GetBDeath())
	{
		return;
	}

	this->SetAffectNavigation(false);
	CharacterInfoComponent->InitCharacterInfo();
	SkillSystemComponent->InitSkillSlot();

	OffSelectCharacter();
	OffViewAllSkillRange();
}

void APlayerCharacter::OffViewAllSkillRange()
{
	ClearPathSplineMeshes();
	DecalAttackRange->SetHiddenInGame(true);
	SkillRangeActor->DeActiveSkillRange();
	DecalPieRange->SetHiddenInGame(true);
}

void APlayerCharacter::OnViewAttackRange()
{
	OffViewAllSkillRange();
	// 현재 고른 스킬
	FSkillStruct skillInfo;

	if (!SkillSystemComponent->GetSelectedSkillInfo(skillInfo))
	{
		return;
	}
	// 캐릭터 중심 원형 사거리 표시
	if (skillInfo.SkillRangeType == ESkillRangeType::CHARACTERCIRCLE)
	{
		GetDecalAttackRange()->SetHiddenInGame(false);
		GetDecalAttackRange()->DecalSize = FVector(200.0f, skillInfo.CharacterRange, skillInfo.CharacterRange);
	}
	else if (skillInfo.SkillRangeType == ESkillRangeType::MOUSECIRCLE)
	{
		// 캐릭터 중심 원형 사거리 표시
		GetDecalAttackRange()->SetHiddenInGame(false);
		GetDecalAttackRange()->DecalSize = FVector(200.0f, skillInfo.CharacterRange, skillInfo.CharacterRange);

		// 마우스 중심 원형 사거리 표시
		SkillRangeActor->ActiveSkillRange(this, skillInfo.MouseRange, skillInfo.TargetType);
		// 타이머로 0.01초마다 스킬 범위 표시
		GetWorld()->GetTimerManager().SetTimer(SkillRangeTimerHandle, this, &APlayerCharacter::ViewSkillRange, 0.01f,
		                                       true);
	}
	else if (skillInfo.SkillRangeType == ESkillRangeType::CHARACTERPIE)
	{
		// 부채꼴 공격일 때는 부채꼴만 ON
		GetDecalPieRange()->SetHiddenInGame(false);
		GetDecalPieRange()->DecalSize = FVector(200.0f, skillInfo.CharacterRange, skillInfo.CharacterRange);
		// 타이머로 0.01초마다 스킬 범위 표시
		GetWorld()->GetTimerManager().SetTimer(SkillRangeTimerHandle, this, &APlayerCharacter::ViewSkillRange, 0.01f,
											   true);
	}
}

void APlayerCharacter::ViewSkillRange()
{
	if (_isControl &&
		GameInstance->GetPlayerCombatController()->CheckPlayerInputState(ClickTarget))
	{
		// 위치 이동
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		FSkillStruct SkillInfo;
		if (!SkillSystemComponent->GetSelectedSkillInfo(SkillInfo))
		{
			return;
		}

		FHitResult Hit;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		MousePos = Hit.ImpactPoint;
		RotateTowardMousePos(MousePos);
		
		// 사거리 안이면 AttackPos 갱신
		if (FVector::Dist(GetActorLocation(), MousePos) <= SkillInfo.CharacterRange)
		{
			SkillSystemComponent->SetSkillPos(MousePos);
		}
		// 사거리 밖에 마우스가 있으면 계산해서 AttackPos 갱신
		else
		{
			// 방향 벡터 계산
			FVector Direction = (MousePos - GetActorLocation()).GetSafeNormal();
			// 사거리 내 최대 위치 계산
			FVector MaxMousePosition = GetActorLocation() + Direction * SkillInfo.CharacterRange;
			SkillSystemComponent->SetSkillPos(MaxMousePosition);
		}
		SkillRangeActor->SetActorLocation(SkillSystemComponent->GetSkillPos());
	}
	else
	{
		SkillRangeActor->DeActiveSkillRange();
		GetWorldTimerManager().ClearTimer(SkillRangeTimerHandle);
	}
}


int32 APlayerCharacter::TakeAttackDamage(int32 DamageNum, bool bPiercingDamage)
{
	// 데미지 처리
	int32 damage = Super::TakeAttackDamage(DamageNum, bPiercingDamage);

	GameInstance->GetInGameUI()->UpdateHP(GetECharacterName());
	return damage;
}

void APlayerCharacter::Respawn()
{
	Super::Respawn();
	CharacterInfoComponent->InitCharacterInfo();
	CharacterInfoComponent->SetCurrentHP(1.0f);

	GameInstance->GetInGameUI()->UpdatePortraitHP(GetECharacterName());
}

void APlayerCharacter::Death()
{
	Super::Death();
	SetIsControl(false);
}

void APlayerCharacter::SkillAction(AGCharacter* target, FVector Pos)
{
	SkillStart(target);
	SkillSystemComponent->UseSkill();
	GetWorldTimerManager().SetTimer(SkillDelayTimerHandle, [this]() { SkillEnd(); }, 4.0f, false);
}

bool APlayerCharacter::CheckUseSkill(int _chooseSikillSlotNum)
{
	// 스킬 번호 확인
	int skillSlotListNum = SkillSystemComponent->GetSkillSlotList().Num();
	if (_chooseSikillSlotNum < 0 || _chooseSikillSlotNum >= skillSlotListNum)
	{
		return false;
	}
	USkillSlot* selectedSkillSlot = SkillSystemComponent->GetSkillSlot(_chooseSikillSlotNum);
	if (!selectedSkillSlot)
	{
		return false;
	}
	USkillBase* mySkill = selectedSkillSlot->GetMySkill();
	if (!mySkill)
	{
		return false;
	}

	if (CharacterInfoComponent->GetCurrentCost() < mySkill->GetCost())
	{
		GameInstance->GetAnnotationWidget()->OnAnnotation(FText(NSLOCTEXT("InGameUIAnnotationText", "Arakadia", "스킬을 사용하기 위한 코스트가 부족합니다!")), 3);
		
		return false;
	}

	// 스킬 쿨타임이면 패쓰
	if (!(mySkill->CheckCurCoolDown()))
	{
		return false;
	}

	return true;
}

void APlayerCharacter::SkillStart(AGCharacter* target)
{
	int cost = SkillSystemComponent->GetSelectedSkillSlot()->GetMySkill()->GetCost();
	
	GameInstance->GetPlayerCombatController()->SetPlayerInputState(ActionSkill, cost);

	// 캐릭터 회전
	if (target)
	{
		RotateTowardTarget(target);
	}
	else
	{
		RotateTowardMousePos(MousePos);
	}
}

void APlayerCharacter::SkillEnd()
{
	// 논타겟인 경우
	if (!SkillSystemComponent->GetSkillTarget())
	{
	}
	// 액션 모드 변경
	if (!GameInstance->GetCombatManager()->CheckCombatState(CombatEnd))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPlayerInputState(ClickSkill)"));
		GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickSkill);
	}
}

void APlayerCharacter::OnPOIColiisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && IsValid(OtherActor->GetComponentByClass(UPointOfInterestComponent::StaticClass())))
	{
		ACharacter* other_character = Cast<ACharacter>(OtherActor);

		if (IsValid(other_character))
		{
			if (GameInstance->GetInGameDataManager()->AddPOIMap(other_character))
			{
				UPointOfInterestComponent* poi_comp = Cast<UPointOfInterestComponent>(
					OtherActor->GetComponentByClass(UPointOfInterestComponent::StaticClass()));
				poi_comp->AddPOI();
			}
		}
	}
}

void APlayerCharacter::GetSkillTexture()
{
	GameInstance->GetInGameUI()->SetSkillButton(SkillSystemComponent->GetSkillSlotList());
}

ESkillRangeType APlayerCharacter::GetSkillRangeType()
{
	return (GetSkillSystemComponent()->GetSelectedSkillSlot()->GetMySkill()->GetSkillInfoManager()->
	                                   GetSkillRangeType());
}

ESkillTargetType APlayerCharacter::GetSelectedSkillTargetType()
{
	return GetSkillSystemComponent()->GetSelectedSkillSlot()->GetMySkill()->GetSkillInfoManager()->GetTargetType();
}

void APlayerCharacter::OnSelectCharacter()
{
	SelectedCharacterLight->SetHiddenInGame(false);
}

void APlayerCharacter::OffSelectCharacter()
{
	SelectedCharacterLight->SetHiddenInGame(true);
}

UMaterial*& APlayerCharacter::GetCircularPortrait()
{
	return circular_portrait;
}

void APlayerCharacter::ClearPathSplineMeshes()
{
	for (auto& splineMesh : PathSplineMeshes)
	{
		splineMesh->DestroyComponent();
	}
	PathSplineMeshes.Empty();
}

UWeaponItemBase* APlayerCharacter::CreateBoundWeapon()
{
	bound_weapon = NewObject<UWeaponItemBase>(GameInstance, bp_bound_weapon);

	return bound_weapon;
}

UWeaponItemBase* APlayerCharacter::GetBoundWeapon()
{
	if(!IsValid(bound_weapon))
	{
		LOG(Error, "기본 장착 무기가 없습니다.");

		return nullptr;
	}
	
	return bound_weapon;
}

void APlayerCharacter::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                       bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	//{
	//	InteractableinRange.AddUnique(OtherActor);
	//}
	AInteractionCharacter* interchar = Cast<AInteractionCharacter>(OtherActor);
	if(interchar)
	{
		//InteractableinRange.AddUnique(interchar->GetDialogComponent());

		ADialogManageActor* adma = interchar->GetDialogManageActor();
		if (!adma)
			return;

		TArray<UActorComponent*> accoarray =
			adma->GetComponentsByInterface(UInteractionInterface::StaticClass());

		for (auto index : accoarray)
		{
			InteractableinRange.AddUnique(index);
		}

		if (InteractableinRange.Num() > 0)
		{
			_isAbleInteract = true;
		}
	}
}

void APlayerCharacter::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	//{
	//	InteractableinRange.Remove(OtherActor);
	//}
	
	AInteractionCharacter* interchar = Cast<AInteractionCharacter>(OtherActor);
	if (interchar)
	{
		//InteractableinRange.Remove(interchar->GetDialogComponent());

		ADialogManageActor* adma = interchar->GetDialogManageActor();

		if (!adma)
			return;

		TArray<UActorComponent*> accoarray =
			adma->GetComponentsByInterface(UInteractionInterface::StaticClass());

		for (auto index : accoarray)
		{
			InteractableinRange.Remove(index);
		}

		//TArray<UActorComponent*> accoarray =
		//	OtherActor->GetComponentsByInterface(UInteractionInterface::StaticClass());
		
		//for (auto index : accoarray)
		//{
		//	InteractableinRange.Remove(index);
		//}
	}
}

void APlayerCharacter::Interact_Pressed()
{
	switch (GetECharacterName())
	{
	case ECharacterName::Cardenio:
		UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Interact_Pressed Cardenio"));
		break;
	case ECharacterName::Fernando:
		UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Interact_Pressed Fernando"));
		break;
	case ECharacterName::Samira:
		UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Interact_Pressed Samira"));
		break;
	case ECharacterName::RuthCinda:
		UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Interact_Pressed RuthCinda:"));
		break;
	}

	if (InteractableinRange.Num() > 0)
	{
		//InteractableinRange[0]->OnInteraction_Implementation(this);
		UDialogComponentBase* DialogComponent = Cast<UDialogComponentBase>(InteractableinRange[0]);
		if (DialogComponent)
		{
			if (DialogComponent->GetOwnerActor()->GetOwnerChar()->GetQuestVisible())
			{
				IInteractionInterface::Execute_OnInteraction(InteractableinRange[0], this);
				GameInstance->GetTutorialManager()->PlayTutorial(ETutorial::Interaction);
			}
			else
			{
				AGPlayerController* PlayerController =
					Cast<AGPlayerController>(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));
				PlayerController->SetInputStateDefault();
			}
		}
		else
		{
			AGPlayerController* PlayerController =
				Cast<AGPlayerController>(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));
			PlayerController->SetInputStateDefault();
		}
	}
	else
	{
		AGPlayerController* PlayerController =
			Cast<AGPlayerController>(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));
		PlayerController->SetInputStateDefault();
	}
}
