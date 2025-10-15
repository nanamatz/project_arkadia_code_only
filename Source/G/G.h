// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 이 함수가 호출된 함수의 이름과 라인을 매크로로 String화 시킴
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// 위의 CALLINFO와 함께 아무 내용이 없는 로그를 찍는 매크로
#define LOG_S(Verbosity) UE_LOG(GLOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)
// CALLINFO와 함께 텍스트로 로그를 찍는 매크로
#define LOG(Verbosity, Format, ...) UE_LOG(GLOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
// Screen에다가 LOG를 직접 찍는 매크로
#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))


DECLARE_LOG_CATEGORY_EXTERN(GLOG, Log, All);
