#include "TutorialWidget.h"

#include "G/G.h"

void UTutorialWidget::RemoveFromParent()
{
	LOG(Log, "Tutorial Remove");
	// OnRemovedFromParent 델리게이트 호출
	OnRemovedFromParent.Broadcast();

	// 부모 클래스의 RemoveFromParent() 호출
	Super::RemoveFromParent();
}

void UTutorialWidget::EndTutorial()
{
	LOG(Log, "End Tutorial");
	// OnRemovedFromParent 델리게이트 호출
	OnRemovedFromParent.Broadcast();
}

void UTutorialWidget::BeginDestroy()
{
	Super::BeginDestroy();
	
	LOG(Log, "UTutorialWidget BeginDestroy");
}
