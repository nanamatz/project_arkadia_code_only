#include "TutorialManager.h"

#include "Blueprint/UserWidget.h"
#include "G/G.h"
#include "G/GGameInstance.h"
#include "G/Manager/DataManager.h"
#include "G/Tutorial/FTutorialType.h"
#include "G/Tutorial/TutorialWidget.h"


UTutorialManager::UTutorialManager()
	: bIsTutorialPlaying(false) // 현재 튜토리얼 재생 여부 초기화
{
}

void UTutorialManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
	_tutorialSet.Reset();
	_tutorialQueue.Empty(); // 튜토리얼 큐 초기화
	bIsTutorialPlaying = false;
	
	LOG(Log, "_tutorialSet : %d", _tutorialSet.Num());
}

void UTutorialManager::SetTutorialSet(TSet<ETutorial> tutorialSet)
{
	for (auto tutorial : tutorialSet)
	{
		_tutorialSet.Add(tutorial);
	}
}

void UTutorialManager::PlayTutorial(ETutorial tutorialType)
{
	// 이미 본 튜토리얼이면 큐에 추가하지 않음
	if (_tutorialSet.Contains(tutorialType))
	{
		LOG(Log, "_tutorialSet : %s는 이미 봤습니다.", *UEnum::GetValueAsString(tutorialType));
		return;
	}

	// 큐에 튜토리얼 타입 추가
	_tutorialQueue.Enqueue(tutorialType);
	LOG(Log, "_tutorialSet : %s 큐에 추가됨.", *UEnum::GetValueAsString(tutorialType));

	// 다른 튜토리얼이 진행 중이 아니라면 다음 튜토리얼을 실행
	if (!bIsTutorialPlaying)
	{
		PlayNextTutorial();
	}
}

void UTutorialManager::PlayNextTutorial()
{
	if (!_tutorialQueue.IsEmpty())
	{
		bIsTutorialPlaying = true;

		ETutorial tutorialType;
		_tutorialQueue.Dequeue(tutorialType); // 큐에서 다음 튜토리얼 꺼내기

		FString tutorialName = UEnum::GetDisplayValueAsText(tutorialType).ToString();
		LOG(Log, "_tutorialSet : %s 튜토리얼 실행.", *tutorialName);
		
		FTutorialType* tutorialStruct = GameInstance->GetDataManager()->GetTutorialData(
			EDataType::TutorialData, tutorialName);

		if (tutorialStruct && tutorialStruct->Type == tutorialType)
		{
			auto newTutorialWidget = CreateWidget<UTutorialWidget>(GameInstance, tutorialStruct->tutorialWidget);
			if(newTutorialWidget){
				newTutorialWidget->AddToViewport();
				_tutorialSet.Add(tutorialType);
			
				// 튜토리얼 완료 후 큐의 다음 튜토리얼 실행
				newTutorialWidget->OnRemovedFromParent.AddDynamic(this, &UTutorialManager::EndCurrentTutorial);
			}
			else
			{
				LOG(Log, "_tutorialSet : %s 튜토리얼 위젯이 없습니다.", *tutorialName);
				PlayNextTutorial(); // 현재 튜토리얼이 없을 경우 다음 큐의 튜토리얼 실행
			}
		}
		else
		{
			LOG(Log, "_tutorialSet : %s 튜토리얼 데이터가 없습니다.", *tutorialName);
			bIsTutorialPlaying = false;
			PlayNextTutorial(); // 현재 튜토리얼이 없을 경우 다음 큐의 튜토리얼 실행
		}
	}
}

void UTutorialManager::EndCurrentTutorial()
{
	bIsTutorialPlaying = false;
	PlayNextTutorial(); // 다음 큐에 있는 튜토리얼 실행
}
