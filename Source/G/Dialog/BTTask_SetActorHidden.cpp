// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetActorHidden.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "G/Dialog/DialogManageActor.h"
#include "G/Dialog/InteractionCharacter.h"
#include "G/Dialog/DialogWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"

UBTTask_SetActorHidden::UBTTask_SetActorHidden()
{
	NodeName = TEXT("SetActorHidden");
}

EBTNodeResult::Type UBTTask_SetActorHidden::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	UGGameInstance* gin = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	const FName DialogWidgetText = TEXT("DialogWidget");
	const FName QuestHashTagText = TEXT("QuestHashTag");

	UE_LOG(LogTemp, Log, TEXT("BTTaskSetActorHidden 32 :: execute"));
	/*UObject* dialogwidget = BlackboardComp->GetValueAsObject(DialogWidgetText);

	if (dialogwidget)
	{
		UE_LOG(LogTemp, Log, TEXT("BTTaskSetActorHidden 37 :: dialogwidget is vaild"));
		UDialogWidgetBase* dialogwidgetbase = Cast<UDialogWidgetBase>(dialogwidget);
		if (dialogwidgetbase)
		{
			UE_LOG(LogTemp, Log, TEXT("BTTaskSetActorHidden 41 :: dialogwidgetbase is vaild"));
			dialogwidgetbase->OwnerActor->GetOwnerChar()->SetQuestHidden();
			dialogwidgetbase->OwnerActor->GetOwnerChar()->SetActorHiddenInGame(true);
		}
	}*/

	FString hashtag = BlackboardComp->GetValueAsString(QuestHashTagText);
	int32 index = gin->QuestManager->GetHashTag(hashtag);

	AInteractionCharacter* interchar = gin->QuestManager->GetQuestList()[index]->AssignedCharacters[NpcNumber];
	if(interchar)
		interchar->SetQuestVisible(Visible);
	else
		gin->QuestManager->GetQuestList()[index]->isVisibles[NpcNumber] = Visible;

	return EBTNodeResult::Succeeded;
}
