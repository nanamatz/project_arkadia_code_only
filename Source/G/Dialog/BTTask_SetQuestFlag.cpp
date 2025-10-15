// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetQuestFlag.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Dialog/FQuestManager.h"
#include "InteractionCharacter.h"
#include "G/Dialog/DialogWidgetBase.h"
#include "G/Dialog/DialogManageActor.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetQuestFlag::UBTTask_SetQuestFlag()
{
	text = "";
}

EBTNodeResult::Type UBTTask_SetQuestFlag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	FName valuename = TEXT("QuestState");
	FName textname = TEXT("QuestHashTag");
	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	text = OwnerComp.GetBlackboardComponent()->GetValueAsString(textname);
	int32 n = gin->QuestManager->GetHashTag(text);

	gin->QuestManager->SetState(n, setnumber);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(valuename, setnumber);

	UE_LOG(LogTemp, Log, TEXT("SetQuestFlag(27) : Hash Tag - %s, Index - %d"), *text, n);

	if (isAccept)
	{
		gin->Quest_AddActiveQuest(n);
		if (!isComplete)
		{
			gin->QuestManager->QuestAcceptAnnounce(n);
		}
	}

	if (isComplete)
	{
		gin->Quest_AddArchivedQuest(n);
		gin->QuestManager->QuestReceiptAnnounce(n);
	}

	const FName DialogWidgetText = TEXT("DialogWidget");

	UE_LOG(LogTemp, Log, TEXT("BTTaskSetQuestFlag 52 :: execute"));
	UObject* dialogwidget = BlackboardComp->GetValueAsObject(DialogWidgetText);

	if (dialogwidget)
	{
		UE_LOG(LogTemp, Log, TEXT("BTTaskSetQuestFlag 57 :: dialogwidget is vaild"));
		UDialogWidgetBase* dialogwidgetbase = Cast<UDialogWidgetBase>(dialogwidget);
		if (dialogwidgetbase)
		{
			UE_LOG(LogTemp, Log, TEXT("BTTaskSetActorHidden 41 :: dialogwidgetbase is vaild"));
			dialogwidgetbase->OwnerActor->GetOwnerChar()->QuestInfoUpdate();
		}
	}

	return EBTNodeResult::Succeeded;
}
