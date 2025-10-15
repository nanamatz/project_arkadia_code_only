// Fill out your copyright notice in the Description page of Project Settings.

#include "NonPlayerCharacter.h"
#include "CombatArea.h"
#include "NPCAIController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "GGameInstance.h"
#include "GGameMode.h"
#include "G/Item/ItemBase.h"
#include <Kismet/GameplayStatics.h>

#include "CharacterInfoComponent.h"
#include "G.h"
#include "NavigationInvokerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "G/Anim/CharacterAnimInstance.h"
#include "Manager/CombatManager.h"
#include "Manager/InventoryManager.h"
#include "Dialog/FQuestManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "UI/Looting/LootingComponent.h"
#include "UI/Map/PointOfInterestComponent.h"

// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	GetMesh()->SetCollisionProfileName(*npcCollisionChannel);
	GetMesh()->SetGenerateOverlapEvents(true);

	//NavigationInvokerComponent->SetGenerationRadii(1024, 512);
	
	// DetectingRange Setting
	DetectingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DetectingCollision"));
	DetectingCollision->SetupAttachment(GetRootComponent());

	DetectingCollision->SetCollisionProfileName("Detecting");
	DetectingCollision->SetCanEverAffectNavigation(false);
	DetectingCollision->SetSphereRadius(DetectingRange);
	DetectingCollision->SetCollisionProfileName("Detecting");
	DetectingCollision->bDynamicObstacle = true;

	DetectingCollision->OnComponentBeginOverlap.
	                    AddDynamic(this, &ANonPlayerCharacter::OnDetectingCollisionBeginOverlap);


	// DetectingRange Setting
	WarningCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WarningCollision"));
	WarningCollision->SetupAttachment(GetRootComponent());

	WarningCollision->SetCollisionProfileName("Detecting");
	WarningCollision->SetCanEverAffectNavigation(false);
	WarningCollision->SetSphereRadius(WarningRange);
	WarningCollision->SetCollisionProfileName("Detecting");
	WarningCollision->bDynamicObstacle = true;

	WarningCollision->OnComponentBeginOverlap.
	                  AddDynamic(this, &ANonPlayerCharacter::OnWarningCollisionBeginOverlap);

	Lootingcomponent = CreateDefaultSubobject<ULootingComponent>(TEXT("LootingComponent"));

	POIComoponent = CreateDefaultSubobject<UPointOfInterestComponent>(TEXT("POIComoponent"));

	static  ConstructorHelpers::FObjectFinder<UTexture2D> POITexture(TEXT("/Game/Resource/ClassicRPGUI/ClassicRPGUI/ClassicUI/BonusIcons/TransparentVersion/demon_icon.demon_icon"));

	if(POITexture.Succeeded())
	{
		POIComoponent->SetIcon(POITexture.Object);
	}
	
	// smooth rotation setting
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ANonPlayerCharacter::SetSpawnNPC(FString NPCName)
{
	BControlling = false;
	
	FCharacterInfo* NPCInfo = GameInstance->GetDataManager()->GetNPCCharacterInfo(NPCName);
	CharacterInfoComponent = NewObject<UCharacterInfoComponent>(this);
	CharacterInfoComponent->SetCharacterInfo(*NPCInfo);
	DetectingCollision->SetSphereRadius(DetectingRange);
	WarningCollision->SetSphereRadius(WarningRange);
	SkillSystemComponent->SetSkillSlot(this, GameInstance->GetDataManager()->GetNPCSkillDataTable(*UEnum::GetDisplayValueAsText(NPCInfo->ENPCName).ToString()));
}

// Called when the game starts or when spawned
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	LOG(Log, "");

	// 게임 인스턴스 저장
	GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	SetSpawnNPC(GetClass()->GetName());
}

// 컨트롤 변경
void ANonPlayerCharacter::SetBControlling(bool b)
{
	// 컨트롤 변경
	BControlling = b;

	ANPCAIController* AiController = Cast<ANPCAIController>(Controller);
	// 컨트롤이 True인 경우 세팅
	if (b)
	{
		// 컨트롤하는 NPC 장애물 처리 해제
		SetAffectNavigation(false);
		AiController->SetCurCostKey(GetMaxCost());
	}
	else
	{
		// 컨트롤 끝난 NPC 장애물 처리
		SetAffectNavigation(true);
	}

	AiController->SetBControlling(b);
}

void ANonPlayerCharacter::SetCombatArea(ACombatArea* CA)
{
	LOG(Log, "");
	if(CA)
	{
		LOG(Log, "Combat Area Nane : %s", *GetName());
		CombatArea = CA;
		ANPCAIController* AiController = GetController<ANPCAIController>();
		
		AiController->SetCombatArea(CombatArea);
		AiController->SetBCombat(false);
	}
	else
	{
		LOG(Log, "Fail Set CombatArea");
	}
}

void ANonPlayerCharacter::SetSpawnLocation(const FVector& location)
{
	SetActorLocation(location);
	ANPCAIController* AiController = GetController<ANPCAIController>();

	if(AiController)
	{
		AiController->SetOriginLocation(location);
	}
}

ACombatArea* ANonPlayerCharacter::GetCombatArea()
{
	return CombatArea;
}

void ANonPlayerCharacter::StartCombat()
{
	if (!GetBDeath() && !GameInstance->GetCombatManager()->CheckCombatTure())
	{
		GameInstance->GetCombatManager()->StartCombat(CombatArea);
	}
}


int32 ANonPlayerCharacter::TakeAttackDamage(int32 DamageNum, bool bPiercingDamage)
{
	// 데미지 처리
	int32 damage = Super::TakeAttackDamage(DamageNum, bPiercingDamage);
	return damage;
}

void ANonPlayerCharacter::Death()
{
	if (GetCharacterInfoManager()->GetBDeath())
	{
		return;
	}
	// 죽음 처리
	Super::Death();
	SetBControlling(false);
	GetMesh()->SetCollisionProfileName("InteractionCollision");

	// 큐에서 찾아서 제거
	// 적 리스트에 저장된 본인 인덱스를 찾아서
	// 큐에 자기 번호를 제거
	GameInstance->GetCombatManager()->DeleteCharacterTurnList(this);

	// 골드 처리
	GameInstance->inventory->AddGold(RewardGold);

	//아이템 처리
	//for (UItemBase* item : RewardItems)
	//{
	//	GameInstance->inventory->AddItem(item);
	//}

	GameInstance->GetInGameDataManager()->GetControlCharacter()->GetCharacterInfoManager()->AddCurrentExp(30);

	FString charname;

	this->GetClass()->GetDefaultObject()->GetName(charname);
	GameInstance->QuestManager->AddGoalKillCount(charname);
}

APlayerCharacter* ANonPlayerCharacter::FindClosestTarget()
{
	//  자기가 컨트롤하는 캐릭터인 경우
	if (this->GetBControlling())
	{
		TArray<APlayerCharacter*> PlayerCharacters = GameInstance->GetInGameDataManager()->GetPlayerCharacterList();
		APlayerCharacter* ReCharacter = nullptr;
		float MinDistance = INFINITY;

		// 가장 가까운 플레이어 찾기
		for (APlayerCharacter* PlayerCharacter : PlayerCharacters)
		{
			// 플레이어 캐릭터가 있을 때
			if (PlayerCharacter && !PlayerCharacter->GetBDeath())
			{
				float CurDistance = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
				if (MinDistance > CurDistance)
				{
					ReCharacter = PlayerCharacter;
					MinDistance = CurDistance;
				}
			}
		}

		if (ReCharacter)
		{
			return ReCharacter;
		}
		return nullptr;
	}

	return nullptr;
}

bool ANonPlayerCharacter::CheckAttackPossibility(AGCharacter* Target, float Radius)
{
	// Radius 안에 타겟이 있는지 확인
	if (GetDistanceTo(Target) <= Radius)
	{
		return true;
	}
	return false;
}

bool ANonPlayerCharacter::TargetAttack(AGCharacter* Target)
{
	// 애니메이션 실행
	auto AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayPrimaryAttackMontage();
	}

	// 데미지 처리
	if (IsValid(Target))
	{
		GetWorldTimerManager().SetTimer(MyDelayTimerHandle, [this, Target]()
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Target);
			if (PlayerCharacter)
			{
				PlayerCharacter->TakeAttackDamage(GetMinDamage());
			}
		}, 1.5f, false);
	}
	return true;
}

void ANonPlayerCharacter::SetRewardItem(TArray<FRewardItemSetting> RewardItemSettingList)
{
	for(const FRewardItemSetting& itemSetting : RewardItemSettingList)
	{
		float dropRate = itemSetting.DropRate;
		float curRate = FMath::RandRange(0, 100);
		//LOG(Log, "curRate : %f", curRate);
		if(curRate <= dropRate && itemSetting.RewardItem != nullptr)
		{
			AddRewardItem(itemSetting.RewardItem);
		}
	}
}

void ANonPlayerCharacter::AddRewardItem(TSubclassOf<UItemBase> RewardItem)
{
	Lootingcomponent->ProvideItemInfoList.Add(RewardItem, 1);
}

void ANonPlayerCharacter::OnDetectingCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                           bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AGCharacter::StaticClass()) && !Cast<AGCharacter>(OtherActor)->GetBDeath())
	{
		StartCombat();
	}
}

void ANonPlayerCharacter::OnWarningCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                         bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGCharacter>())
	{
		Cast<ANPCAIController>(GetController())->SetBCombat(true);
		RotateTowardTarget(Cast<AGCharacter>(OtherActor));
		LOG(Log, "Warning");
	}
}
void ANonPlayerCharacter::OpenLootingUI(FVector2D pos)
{
	Lootingcomponent->OpenLootingUI(pos);
}

void ANonPlayerCharacter::CloseLootingUI()
{
	Lootingcomponent->CloseLootingUI();
}

bool ANonPlayerCharacter::isLootingEmpty()
{
	return Lootingcomponent->isEmpty();
}