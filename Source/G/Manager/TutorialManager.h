#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "G/Util/Define.h"
#include "TutorialManager.generated.h"

UCLASS()
class UTutorialManager : public UManagerBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSet<ETutorial> _tutorialSet;

	TQueue<ETutorial> _tutorialQueue;

	bool bIsTutorialPlaying;

public:
	UTutorialManager();
	
	virtual void Init(UGGameInstance* gameInstance) override;
	
	TSet<ETutorial> GetTutorialSet() {return _tutorialSet; }
	
	void SetTutorialSet(TSet<ETutorial> tutorialSet);

	UFUNCTION(BlueprintCallable)
	void PlayTutorial(ETutorial tutorialType);
	void PlayNextTutorial();

	UFUNCTION()
	void EndCurrentTutorial();
};
