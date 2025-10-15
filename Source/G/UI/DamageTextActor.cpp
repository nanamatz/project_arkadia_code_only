// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/DamageTextActor.h"
#include <G/UI/DamageTextUI.h>
#include "Components/WidgetComponent.h"
#include "G/GGameInstance.h"
#include "G/Manager/ResourceManager.h"
#include "G/Manager/PoolManager/Poolable.h"
#include "Kismet/GameplayStatics.h"

class UGGameInstance;
// Sets default values
ADamageTextActor::ADamageTextActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PoolableComponent = CreateDefaultSubobject<UPoolable>(TEXT("Poolable"));

	static ConstructorHelpers::FClassFinder<UDamageTextUI> WidgetClassFinder(
		TEXT("WidgetBlueprint'/Game/UI/GameUI/BP_CombatDamageTextWidget.BP_CombatDamageTextWidget_C'"));
	DamageTextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
	if (WidgetClassFinder.Succeeded())
	{
		DamageTextWidgetComponent->SetWidgetClass(WidgetClassFinder.Class);
		DamageTextWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}

	SetRootComponent(DamageTextWidgetComponent);
}

void ADamageTextActor::Init(FVector _startLocation, float _damage, EDamageType damageType)
{
	SetActorLocation(_startLocation);

	Cast<UDamageTextUI>(DamageTextWidgetComponent->GetWidget())->Init(this, _damage, damageType);

	GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle, [&]
	                                       {
		                                       UGGameInstance* GameInstance = Cast<UGGameInstance>(
			                                       UGameplayStatics::GetGameInstance(GetWorld()));
		                                       GameInstance->GetResourceManager()->ObjectDestory(this);
	                                       }
	                                       , 1.5f, false);
}

void ADamageTextActor::Init(FVector _startLocation, FText text)
{
	SetActorLocation(_startLocation);

	Cast<UDamageTextUI>(DamageTextWidgetComponent->GetWidget())->Init(this, text);

	GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle, [&]
										   {
											   UGGameInstance* GameInstance = Cast<UGGameInstance>(
												   UGameplayStatics::GetGameInstance(GetWorld()));
											   GameInstance->GetResourceManager()->ObjectDestory(this);
										   }
										   , 1.5f, false);
}
