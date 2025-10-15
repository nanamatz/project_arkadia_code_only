// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class G_API ULootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	class UGGameInstance* GameInstance;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//void init(class UGGameInstance* gin);
	void OpenLootingUI(FVector2D pos);
	void CloseLootingUI();
	bool isEmpty() { return ProvideItemInfoList.Num() == 0; }


	UPROPERTY(EditAnywhere, Category = "Reward")
	TMap<TSubclassOf<class UItemBase>, int32> ProvideItemInfoList;
};
