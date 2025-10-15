// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/NPCAIController.h"
#include "AIController_BuffGreen.generated.h"

/**
 * 
 */
UCLASS()
class G_API AAIController_BuffGreen : public ANPCAIController
{
	GENERATED_BODY()

	// BT, BB 세팅
	AAIController_BuffGreen();

	// 해당 NPC가 필요한 키 값들
public:
	static const FName CanBoomKey; // 터질 수 있는지

	// 애니메이션 수행 후 사망
	void SkillBoom();
};
