// Fill out your copyright notice in the Description page of Project Settings.


#include "G/Dialog/BTTask_SetQuestProgress.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Dialog/FQuestManager.h"
#include "InteractionCharacter.h"
#include "G/Dialog/DialogWidgetBase.h"
#include "G/Dialog/DialogManageActor.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestProgress::UBTTask_SetQuestProgress()
{
	NodeName = TEXT("SetQuestProgress");
}

EBTNodeResult::Type UBTTask_SetQuestProgress::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	int32 n = gin->QuestManager->GetHashTag(HashTag);

	EQuestProgress prevProgress = gin->QuestManager->GetQuestProgress(n);

	gin->QuestManager->SetQuestProgress(n, Progress);

	UE_LOG(LogTemp, Log, TEXT("SetQuestProgress(36) : Hash Tag - %s, Index - %d"), *HashTag, n);

	if (prevProgress == EQuestProgress::NotExpose && Progress == EQuestProgress::Ready)
	{
		gin->QuestManager->GetQuestList()[n]->isVisibles[0] = true;
	}
	else if (prevProgress == EQuestProgress::Ready && Progress == EQuestProgress::OnGoing)
	{
		gin->QuestManager->QuestAcceptAnnounce(n);
		gin->Quest_AddActiveQuest(n);
	}
	else if (prevProgress == EQuestProgress::OnGoing && Progress == EQuestProgress::Complete)
	{
		gin->QuestManager->QuestAchieveAnnounce(n);
	}
	else if (prevProgress == EQuestProgress::OnGoing && Progress == EQuestProgress::End)
	{
		gin->QuestManager->QuestReceiptAnnounce(n);
		gin->Quest_AddArchivedQuest(n);
	}
	else if (prevProgress == EQuestProgress::Complete && Progress == EQuestProgress::End)
	{
		gin->QuestManager->QuestReceiptAnnounce(n);
		gin->Quest_AddArchivedQuest(n);
	}

	if (State != -1)
	{
		gin->QuestManager->SetState(n, State);
	}
	

	for (auto iter : gin->QuestManager->GetQuestList()[n]->AssignedCharacters)
	{
		if (iter)
		{
			iter->QuestInfoUpdate();
		}
	}

	return EBTNodeResult::Succeeded;
}
