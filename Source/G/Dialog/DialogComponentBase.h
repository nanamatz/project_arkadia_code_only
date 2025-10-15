// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogComponentBase.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class G_API UDialogComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDialogComponentBase();

protected:
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	class UDialogWidgetBase* DialogWidgetBase;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class ADialogManageActor* OwnerActor;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* DialogTreeBase;

public:
	void Init(ADialogManageActor* oic) { OwnerActor = oic; }
	void SetDialogTreeBase(UBehaviorTree* dt) { DialogTreeBase = dt; }

	UDialogWidgetBase* GetDialogWidget() { return DialogWidgetBase; }
	ADialogManageActor* GetOwnerActor() { return OwnerActor; }

	UFUNCTION(BlueprintCallable)
	void DialogWidgetBaseSetOwnerActor();
};
