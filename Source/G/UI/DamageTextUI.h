// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "G/Util/Define.h"
#include "DamageTextUI.generated.h"

/**
 * 
 */
UCLASS()
class G_API UDamageTextUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* DefaultPopupAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShieldPopupAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* CriticalPopupAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MissPopupAnimation;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageTextBlock;

	FTimerHandle animationTimer;

public:
	void Spawn();
	void Init(AActor* actor, float _damage, bool isShield = false);
	void Init(AActor* actor, float _damage, EDamageType damageType);
	void Init(AActor* actor, FText text);
};
