// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetQuestFlag.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "G/Dialog/FQuestManager.h"
#include "G/Dialog/InteractionCharacter.h"
#include "G/Dialog/DialogWidgetBase.h"
#include "G/Dialog/DialogManageActor.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetQuestFlag::UBTTask_GetQuestFlag()
{
	text = "";
	getnumber = -5;
}

EBTNodeResult::Type UBTTask_GetQuestFlag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	FName valuename = TEXT("QuestState");
	FName textname = TEXT("QuestHashTag");
	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	text = OwnerComp.GetBlackboardComponent()->GetValueAsString(textname);
	UE_LOG(LogTemp, Log, TEXT("GetQuestFlag key: %s"), *text);
	int32 n = gin->QuestManager->GetHashTag(text);

	if (n == -1)
	{
		UE_LOG(LogTemp, Error, TEXT("GetQuestFlag Error! : GethashTag is return -1. text is %s"), *text);
		return EBTNodeResult::Aborted;
	}

	getnumber = gin->QuestManager->GetState(n);
	UE_LOG(LogTemp, Log, TEXT("GetQuestFlag state: %d"), getnumber);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(valuename, getnumber);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	const FName DialogWidgetText = TEXT("DialogWidget");

	UE_LOG(LogTemp, Log, TEXT("GetQuestFlag 41 :: execute"));
	UObject* dialogwidget = BlackboardComp->GetValueAsObject(DialogWidgetText);

	if (dialogwidget)
	{
		UE_LOG(LogTemp, Log, TEXT("BTTaskSetActorHidden 37 :: dialogwidget is vaild"));
		UDialogWidgetBase* dialogwidgetbase = Cast<UDialogWidgetBase>(dialogwidget);
		if (dialogwidgetbase)
		{
			UE_LOG(LogTemp, Log, TEXT("BTTaskSetActorHidden 41 :: dialogwidgetbase is vaild"));

			gin->QuestManager->GetQuestList()[n]->AssignedCharacters[dialogwidgetbase->OwnerActor->GetOwnerChar()->NpcNumber]
				= dialogwidgetbase->OwnerActor->GetOwnerChar();
			dialogwidgetbase->OwnerActor->GetOwnerChar()->QuestInfoUpdate();
		}
	}


	return EBTNodeResult::Succeeded;
}
