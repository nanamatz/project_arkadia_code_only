#pragma once
#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "TutorialWidget.generated.h"

// 위젯 제거 이벤트 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemovedFromParentEvent);

UCLASS(BlueprintType)
class UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯이 제거될 때 호출되는 델리게이트 이벤트
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnRemovedFromParentEvent OnRemovedFromParent;

protected:
	// RemoveFromParent가 호출될 때마다 OnRemovedFromParent 이벤트를 실행
	virtual void RemoveFromParent() override;

	UFUNCTION(BlueprintCallable)
	void EndTutorial();

	UFUNCTION()
	virtual void BeginDestroy() override;
};