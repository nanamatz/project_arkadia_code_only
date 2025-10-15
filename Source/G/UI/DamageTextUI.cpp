// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/DamageTextUI.h"
#include <Components/TextBlock.h>

#include "Animation/UMGSequencePlayer.h"
#include "G/G.h"
#include "G/Util/Define.h"


void UDamageTextUI::Spawn()
{
}

void UDamageTextUI::Init(AActor* actor, float _damage, bool isShield)
{
	const FString damageString = FString::Printf(TEXT("%.2f"), _damage);
	DamageTextBlock->SetText(FText::FromString(damageString));

	if (isShield)
	{
		PlayAnimation(ShieldPopupAnimation);
	}
	else
	{
		PlayAnimation(DefaultPopupAnimation);
	}
}

void UDamageTextUI::Init(AActor* actor, float _damage, EDamageType damageType = Default)
{
	FString damageString = FString::Printf(TEXT("%.2f"), _damage);
	switch (damageType)
	{
	case Default:
		damageString = FString::Printf(TEXT("%.2f"), _damage);
		break;
	case Critical:
		damageString = FString::Printf(TEXT("%.2f"), _damage);
		break;
	case Shield:
		damageString = FString::Printf(TEXT("%.2f"), _damage);
		break;
	case Miss:
		damageString = FString::Printf(TEXT("빗나감"));
		break;
	}
	DamageTextBlock->SetText(FText::FromString(damageString));

	switch (damageType)
	{
	case Default:
		PlayAnimation(DefaultPopupAnimation);
		break;
	case Critical:
		PlayAnimation(CriticalPopupAnimation);
		break;
	case Shield:
		PlayAnimation(ShieldPopupAnimation);
		break;
	case Miss:
		PlayAnimation(MissPopupAnimation);
		break;
	}
}

void UDamageTextUI::Init(AActor* actor, FText text)
{
	DamageTextBlock->SetText(text);
	PlayAnimation(DefaultPopupAnimation);
}
