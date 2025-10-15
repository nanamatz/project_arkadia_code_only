// Fill out your copyright notice in the Description page of Project Settings.


#include "FQuestManager.h"
#include "FQuestData.h"
#include "G/UI/QuestUI.h"
#include "G/Item/ItemBase.h"
#include "G/GGameInstance.h"
#include "G/Manager/InventoryManager.h"
#include "G/NonPlayerCharacter.h"
#include "G/Dialog/InteractionCharacter.h"
#include "G/G.h"
#include "G/Save/PlayerCharacterInfoSaveGame.h"
#include "G/Manager/InGameDataManager.h"
#include "G/UI/InGameUI/AnnotationWidget.h"

UQuestManager::UQuestManager()
{
	QuestArray.Empty();

	DoingQuestArray.Empty();

	EndQuestArray.Empty();

	QuestHashTagArray.Empty();
	QuestDataTable = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable> QuestDataTableFinder(
		TEXT("DataTable'/Game/DialogSystem/QuestDataTable.QuestDataTable'"));
	UE_LOG(LogTemp, Log, TEXT("UQuestmanager :: init execute"));
	if (QuestDataTableFinder.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestmanager :: init execute succeeded"));
		QuestDataTable = Cast<UDataTable>(QuestDataTableFinder.Object);
	}
}

UQuestManager::~UQuestManager()
{
	/*for (auto iter : QuestArray)
	{
		delete iter;
	}
	QuestArray.Empty();
	DoingQuestArray.Empty();
	EndQuestArray.Empty();*/
}

void UQuestManager::Init(UGGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

void UQuestManager::GenerateQuestInfo(UGGameInstance* ins)
{
	GameInstance = ins;
	if (QuestDataTable)
	{
		TArray<FName> RowNames = QuestDataTable->GetRowNames();
		FQuestData* questdata;

		for (auto RowName : RowNames)
		{
			UE_LOG(LogTemp, Log, TEXT("UQuestmanager :: init execute %s"), *(RowName.ToString()));
			questdata = QuestDataTable->FindRow<FQuestData>(RowName, RowName.ToString());
			UQuestInfo* newQuestInfo = NewObject<UQuestInfo>(this, UQuestInfo::StaticClass());

			newQuestInfo->init(questdata->Name, questdata->Content);

			if (questdata->isReady)
			{
				newQuestInfo->QuestProgress = EQuestProgress::Ready;
				newQuestInfo->isVisibles[0] = true;
			}
			else
			{
				newQuestInfo->QuestProgress = EQuestProgress::NotExpose;
				newQuestInfo->isVisibles[0] = false;
			}

			QuestArray.Add(newQuestInfo);
			QuestHashTagArray.Add(questdata->HashTag);

			GameInstance->GQuestArray.Add(newQuestInfo);
			GameInstance->GQuestHashTagArray.Add(questdata->HashTag);
		}
	}

	/*UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: QuestArray.Num() :: %d"), QuestArray.Num());

	for (int32 i = 0; i < QuestArray.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: QuestArray %d name - %s, content - %s"),
			i, *(QuestArray[i]->Name), *(QuestArray[i]->Content));
	}

	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: DoingQuestArray.Num() :: %d"), DoingQuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: EndQuestArray.Num() :: %d"), EndQuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: hashtagArray.Num() :: %d"), QuestHashTagArray.Num());
	for (int32 i = 0; i < QuestHashTagArray.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: hashtagArray %d tag - %s"),
			i, *(QuestHashTagArray[i]));
	}


	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: GQuestArray.Num() :: %d"), GameInstance->GQuestArray.Num());

	for (int32 i = 0; i < GameInstance->GQuestArray.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: GQuestArray %d name - %s, content - %s"),
			i, *(GameInstance->GQuestArray[i]->Name), *(GameInstance->GQuestArray[i]->Content));
	}

	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: GDoingQuestArray.Num() :: %d"), GameInstance->GDoingQuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: GEndQuestArray.Num() :: %d"), GameInstance->GEndQuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: GhashtagArray.Num() :: %d"), GameInstance->GQuestHashTagArray.Num());
	for (int32 i = 0; i < GameInstance->GQuestHashTagArray.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestInfo Init :: GhashtagArray %d tag - %s"),
			i, *(GameInstance->GQuestHashTagArray[i]));
	}*/
}

int32 UQuestManager::GetHashTag(FString str)
{
	/*
	for (int32 i = 0; i < QuestArray.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestManager GetHashtag:: QuestArray index - %d, address - %d"),i, QuestArray[i]);
		//UE_LOG(LogTemp, Log, TEXT("UQuestManager GetHashtag:: GQuestArray index - %d, address - %d"), i, GameInstance->GQuestArray[i]);
		if (QuestArray[i] != nullptr)
		{
			//UE_LOG(LogTemp, Log, TEXT("UQuestManager GetHashtag :: QuestArray %d name - %s, content - %s"),
			//	i, *(QuestArray[i]->Name), *(QuestArray[i]->Content));
			UE_LOG(LogTemp, Log, TEXT("UQuestManager GetHashtag :: QuestArray %d, arrayindex - %d, progress - %s"),
				i, QuestArray[i]->ArrayIndex, *GetEnumName(QuestArray[i]->QuestProgress));
			//UE_LOG(LogTemp, Log, TEXT("UQuestManager GetHashtag :: GQuestArray %d, arrayindex - %d, progress - %s"),
			//	i, GameInstance->GQuestArray[i]->ArrayIndex, *GetEnumName(GameInstance->GQuestArray[i]->QuestProgress));
		}
	}
	UE_LOG(LogTemp, Log, TEXT("UQuestManager GethashTag :: QuestArray.Num() :: %d"), QuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestManager GethashTag :: DoingQuestArray.Num() :: %d"), DoingQuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestManager GethashTag :: EndQuestArray.Num() :: %d"), EndQuestArray.Num());
	UE_LOG(LogTemp, Log, TEXT("UQuestManager GethashTag :: hashtagArray.Num() :: %d"), QuestHashTagArray.Num());
	for (int32 i = 0; i < QuestHashTagArray.Num(); i++)
	{
		//UE_LOG(LogTemp, Log, TEXT("UQuestManager Gethashtag :: ㅎhashtagArray %d, index - %d, tag - %s"),
		//	&(GameInstance->GQuestHashTagArray[i]), i, *(GameInstance->GQuestHashTagArray[i]));
		UE_LOG(LogTemp, Log, TEXT("UQuestManager Gethashtag :: hashtagArray %d, index - %d, tag - %s"),
			&(QuestHashTagArray[i]), i, *(QuestHashTagArray[i]));
	}
	*/

	//for (int32 i = 0; i < QuestArray.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("UQuestManager Gethashtag :: QuestArray %d, index - %d, Name - %s"),
	//		QuestArray[i], i, *(QuestArray[i]->Name));

	//	for (int j = 0; j < 5; j++)
	//	{
	//		if (QuestArray[i]->isVisibles[j])
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("UQuestManager Gethashtag :: %d th isVisble is true"), j);
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("UQuestManager Gethashtag :: %d th isVisble is false"), j);
	//		}
	//	}
	//}
	//위가 디버깅용 코드
	//아래는 원본

	for (int32 i = 0; i < QuestHashTagArray.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("GetHashTag :: hashtagArray :: %s"), *QuestHashTagArray[i]);
		if (str.Equals(QuestHashTagArray[i]))
		{
			return i;
		}
	}
	return -1;
}

void UQuestManager::QuestDo(int32 index)
{
	QuestArray[index]->isDo = true;
	QuestArray[index]->ArrayIndex = DoingQuestArray.Num();
	QuestArray[index]->QuestProgress = EQuestProgress::OnGoing;
	DoingQuestArray.Add(QuestArray[index]);

	for (auto iter : QuestArray[index]->AssignedCharacters)
	{
		if (iter)
		{
			iter->QuestInfoUpdate();
		}
	}
	UE_LOG(LogTemp, Log, TEXT("UQuestManager(45) QuestDo Execute : Index - %d, Title - %s, Content - %s"),
		index, *QuestArray[index]->Name, *QuestArray[index]->Content);
}

void UQuestManager::QuestComplete(int32 index)
{
	QuestArray[index]->isDo = false;
	QuestArray[index]->isCompletion = true;
	QuestArray[index]->QuestProgress = EQuestProgress::End;

	for (auto iter : QuestArray[index]->AssignedCharacters)
	{
		if (iter)
		{
			iter->QuestInfoUpdate();
		}
	}

	for (int32 i = QuestArray[index]->ArrayIndex + 1; i < DoingQuestArray.Num(); i++)
	{
		DoingQuestArray[i]->ArrayIndex -= 1;
	}

	DoingQuestArray.Remove(QuestArray[index]);
	QuestArray[index]->ArrayIndex = EndQuestArray.Num();
	EndQuestArray.Add(QuestArray[index]);
}

#define LOCTEXT_NAMESPACE "QuestManager"

void UQuestManager::QuestAcceptAnnounce(int32 index)
{
	UE_LOG(LogTemp, Log, TEXT("UQUestmanager::QuestAcceptAnnounce execute"));
	FString str;
	//str.Append(TEXT("퀘스트 \""));
	//str.Append(QuestArray[index]->Name);
	//str.Append(TEXT("\" 수락"));
	//FText AnnotationText = FText::FromString(str);

	FText AnnotationText =
		FText::Format(LOCTEXT("QuestManager_Announce", "퀘스트 \"{0}\" 수락"), FText::FromString((QuestArray[index]->Name)));
	GameInstance->GetAnnotationWidget()->OnAnnotation(AnnotationText, 5);
}

void UQuestManager::QuestAchieveAnnounce(int32 index)
{
	UE_LOG(LogTemp, Log, TEXT("UQUestmanager::QuestAchieveAnnounce execute"));
	FText AnnotationText =
		FText::Format(LOCTEXT("QuestManager_Announce", "퀘스트 \"{0}\" 조건 달성"), FText::FromString((QuestArray[index]->Name)));
	GameInstance->GetAnnotationWidget()->OnAnnotation(AnnotationText, 5);
}

void UQuestManager::QuestReceiptAnnounce(int32 index)
{
	UE_LOG(LogTemp, Log, TEXT("UQUestmanager::QuestReceiptAnnounce execute"));
	FText AnnotationText =
		FText::Format(LOCTEXT("QuestManager_Announce", "퀘스트 \"{0}\" 완료"), FText::FromString((QuestArray[index]->Name)));
	GameInstance->GetAnnotationWidget()->OnAnnotation(AnnotationText, 5);
}

#undef LOCTEXT_NAMESPACE

void UQuestManager::QuestGoalInitialize(int32 index)
{
	QuestArray[index]->QuestInitialize();
}

void UQuestManager::SetQuestGoalRequiredItem(int32 index, TMap<class UItemBase*, int32> Item_Map)
{
	QuestArray[index]->GoalType = QuestGoalType::Item_Required;
	for (auto iter : Item_Map)
	{
		QuestArray[index]->RequiredItemsList.Add(iter.Key->Name.ToString(), iter.Value);
	}

	//QuestArray[index]->RequiredItemsList = Item_Map;
}

void UQuestManager::SetQuestGoalKillEnemy(int32 index, TMap<class ANonPlayerCharacter*, int32> Enemy_Map)
{
	QuestArray[index]->GoalType = QuestGoalType::Kill_Enemy;
	FString name;
	for (auto iter : Enemy_Map)
	{
		iter.Key->GetName(name);
		QuestArray[index]->KillEnemyList.Add(name, iter.Value);
	}

	//QuestArray[index]->KillEnemyList = Enemy_Map;
	QuestArray[index]->EnemyKillCount.Empty();

	for (auto iter : QuestArray[index]->KillEnemyList)
	{
		QuestArray[index]->EnemyKillCount.Add(0);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager(87) : Enemykillcount Add - %d"),
			QuestArray[index]->EnemyKillCount[
				QuestArray[index]->EnemyKillCount.Num() - 1]);
	}

	UE_LOG(LogTemp, Log, TEXT("UQuestManager(91) : Enemykillcount Num - %d"),
		QuestArray[index]->EnemyKillCount.Num());
}

void UQuestManager::SetQuestGoalReachPosition(int32 index)
{
	QuestArray[index]->GoalType = QuestGoalType::Reach_Position;
}

bool UQuestManager::CheckQuestGoalComplete(int32 index)
{
	UQuestInfo* Quest = QuestArray[index];

	bool isClear = true;

	if (Quest->GoalType == QuestGoalType::Item_Required)
	{
		for (auto iter : Quest->RequiredItemsList)
		{
			if (iter.Value > GameInstance->GetInventoryManager()->GetItemCount(FText::FromString(iter.Key)))
			{
				isClear = false;
			}
		}
	}
	else if (Quest->GoalType == QuestGoalType::Kill_Enemy)
	{
		int i = 0;
		for (auto iter : Quest->KillEnemyList)
		{
			if (iter.Value > Quest->EnemyKillCount[i])
			{
				isClear = false;
			}
			UE_LOG(LogTemp, Log, TEXT("CheckGoalClear(50) : value - %d, Killcount - %d"),
				iter.Value, Quest->EnemyKillCount[i]);
			i++;
		}
	}
	else
	{
		isClear = false;
	}

	return isClear;
}

void UQuestManager::AddGoalKillCount(FString targetname)
{
	FString checkname;
	bool changed = false;
	TArray<UQuestInfo*> completeautoquest;
	for (auto iter : DoingQuestArray)
	{
		changed = false;
		if (iter->GoalType == QuestGoalType::Kill_Enemy)
		{
			int i = 0;
			for (auto kter : iter->KillEnemyList)
			{
				checkname = kter.Key;
				//kter.Key->GetName(checkname);
				UE_LOG(LogTemp, Log, TEXT("FQuestMaanger(165) : targetname - %s, checkname - %s"), *targetname,
					*checkname);

				if (checkname.Equals(targetname))
				{
					iter->EnemyKillCount[i] += 1;
					changed = true;
					UE_LOG(LogTemp, Log, TEXT("FQuestMaanger(170) : Enemykillcount - %d"),
						iter->EnemyKillCount[i]);
				}
				UE_LOG(LogTemp, Log, TEXT("FQuestMaanger(173) : Enemykillcount - %d"),
					iter->EnemyKillCount[i]);
				i++;
			}
		}
		if (changed && CheckQuestGoalComplete(QuestArray.Find(iter)))
		{
			iter->QuestProgress = EQuestProgress::Complete;
			for (auto jter : iter->AssignedCharacters)
			{
				if (jter)
				{
					jter->QuestInfoUpdate();
				}
				if (!iter->isAutomationSet)
				{
					QuestAchieveAnnounce(QuestArray.Find(iter));
				}
			}

			if (iter->isAutomationSet)
			{
				completeautoquest.Add(iter);
			}
		}
	}

	for (UQuestInfo* iter : completeautoquest)
	{
		QuestReceiptAnnounce(QuestArray.Find(iter));
		QuestRewardPayment(QuestArray.Find(iter));
		GameInstance->Quest_AddArchivedQuest(QuestArray.Find(iter));

		for (auto jter : iter->RequiredItemsList)
		{
			GameInstance->inventory->DeleteItem(
				GameInstance->inventory->GetItemByName(FText::FromString(jter.Key)), jter.Value);
		}
	}
}

void UQuestManager::ChangeGoalItemCount(UItemBase* targetitem)
{
	FString checkname;
	bool changed = false;
	TArray<UQuestInfo*> completeautoquest;
	for (auto iter : DoingQuestArray)
	{
		changed = false;
		if (iter->GoalType == QuestGoalType::Item_Required)
		{
			int i = 0;
			for (auto kter : iter->RequiredItemsList)
			{
				//UE_LOG(LogTemp, Log, TEXT("FQuestMaanger(165) : targetname - %s, checkname - %s"), *targetname, *checkname);
				//if (kter.Key->Name.EqualTo(targetitem->Name))
				if (kter.Key.Equals(targetitem->Name.ToString()))
				{
					changed = true;
				}
			}
			i++;
		}
		if (changed)
		{
			if (CheckQuestGoalComplete(QuestArray.Find(iter)))
			{
				iter->QuestProgress = EQuestProgress::Complete;
				for (auto jter : iter->AssignedCharacters)
				{
					if (jter)
					{
						jter->QuestInfoUpdate();
					}
					if (!iter->isAutomationSet)
						QuestAchieveAnnounce(QuestArray.Find(iter));
				}

				if (iter->isAutomationSet)
				{
					completeautoquest.Add(iter);
				}
			}
			else
			{
				iter->QuestProgress = EQuestProgress::OnGoing;
				for (auto jter : iter->AssignedCharacters)
				{
					if (jter)
					{
						jter->QuestInfoUpdate();
					}
				}
			}
		}
	}
	
	for (UQuestInfo* iter : completeautoquest)
	{
		QuestReceiptAnnounce(QuestArray.Find(iter));
		QuestRewardPayment(QuestArray.Find(iter));
		GameInstance->Quest_AddArchivedQuest(QuestArray.Find(iter));

		for (auto jter : iter->RequiredItemsList)
		{
			GameInstance->inventory->DeleteItem(
				GameInstance->inventory->GetItemByName(FText::FromString(jter.Key)), jter.Value);
		}
	}

}

void UQuestManager::AchieveReachPositionIndex(int32 index)
{
	if (QuestArray[index]->GoalType == QuestGoalType::Reach_Position)
	{
		if (QuestArray[index]->isAutomationSet)
		{
			QuestReceiptAnnounce(index);
			QuestRewardPayment(index);
		}
		GameInstance->Quest_AddArchivedQuest(index);
	}
}

void UQuestManager::AchieveReachPositionHashTag(FString tag)
{
	int32 index = GetHashTag(tag);
	if (QuestArray[index]->GoalType == QuestGoalType::Reach_Position)
	{
		if (QuestArray[index]->isAutomationSet)
		{
			QuestReceiptAnnounce(index);
			QuestRewardPayment(index);
		}
		GameInstance->Quest_AddArchivedQuest(index);
	}

}

void UQuestManager::SetQuestAutomationComplete(int32 index, TMap<UItemBase*, int32> RewardItemsList, int32 RewardGold, int32 RewardExp, int32 RewardState)
{
	if (QuestArray[index]->isAutomationSet)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestManager::SetQuestAutomationComplete 188 - isAutomationSet is already true, Be careful as there is a possibility that the quest rewards will be overwritten"));
	}

	QuestArray[index]->isAutomationSet = true;
	QuestArray[index]->CompleteRewardItemsList = RewardItemsList;
	QuestArray[index]->CompleteRewardGold = RewardGold;
	QuestArray[index]->CompleteRewardExp = RewardExp;
	QuestArray[index]->CompleteSetState = RewardState;

	//여기에 아이템 받는 완료 조건을 추가할 것

	if (CheckQuestGoalComplete(index))
	{
		QuestArray[index]->QuestProgress = EQuestProgress::Complete;

		QuestReceiptAnnounce(index);
		QuestRewardPayment(index);
		GameInstance->Quest_AddArchivedQuest(index);

		for (auto jter : QuestArray[index]->RequiredItemsList)
		{
			GameInstance->inventory->DeleteItem(
				GameInstance->inventory->GetItemByName(FText::FromString(jter.Key)), jter.Value);
		}
	}
	return;
}

void UQuestManager::QuestRewardPayment(int32 index)
{
	UE_LOG(LogTemp, Log, TEXT("UQuestManager::QuestRewardPayment execute"));
	int32 j = 0;
	for (TTuple<UItemBase*, int32> item_tuple : QuestArray[index]->CompleteRewardItemsList)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestManager::QuestRewardPayment item_tuple %d"), j++);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager::QuestRewardPayment item %d"), item_tuple.Key);
		for (int i = 0; i < item_tuple.Value; i++)
		{

			if (item_tuple.Key)
			{
				UE_LOG(LogTemp, Log, TEXT("UQuestManager::QuestRewardPayment item %s"), *(item_tuple.Key->Name.ToString()));
				GameInstance->inventory->AddItem(item_tuple.Key);
			}
		}

	}

	GameInstance->inventory->AddGold(QuestArray[index]->CompleteRewardGold);

	for (auto iter : GameInstance->GetInGameDataManager()->GetPlayerCharacterList())
	{
		if (iter->GetCharacterInfoManager())
			iter->GetCharacterInfoManager()->AddCurrentExp(QuestArray[index]->CompleteRewardExp);
	}

	if(QuestArray[index]->CompleteSetState != -1)
		QuestArray[index]->State = QuestArray[index]->CompleteSetState;
}

void UQuestManager::AllItemGoalQuestSetOnGoing()
{
	for (auto iter : DoingQuestArray)
	{
		if (iter->GoalType == QuestGoalType::Item_Required)
		{
			iter->QuestProgress = EQuestProgress::OnGoing;
		}
	}
}

void UQuestManager::SaveQuestManager(TArray<FQuestInfoSaveData>& questarray, TArray<FQuestInfoSaveData>& doingquestarray,
	TArray<FQuestInfoSaveData>& endquestarray, TArray<FString>& hashtagarray, TArray<int32>& goaltypearray,
	TArray<int32>& progressarray)
{
	UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager Num :: QuestArray - %d, DOing - %d, End - %d, hashtag - %d"),
		QuestArray.Num(), DoingQuestArray.Num(), EndQuestArray.Num(), QuestHashTagArray.Num());
	FQuestInfoSaveData newQuestInfo;
	newQuestInfo.isVisibles.Add(false);
	newQuestInfo.isVisibles.Add(false);
	newQuestInfo.isVisibles.Add(false);
	newQuestInfo.isVisibles.Add(false);
	newQuestInfo.isVisibles.Add(false);

	newQuestInfo.AssignedCharacters.Add(nullptr);
	newQuestInfo.AssignedCharacters.Add(nullptr);
	newQuestInfo.AssignedCharacters.Add(nullptr);
	newQuestInfo.AssignedCharacters.Add(nullptr);
	newQuestInfo.AssignedCharacters.Add(nullptr);

	for (auto iter : QuestArray)
	{
		newQuestInfo.Name = iter->Name;
		newQuestInfo.Content = iter->Content;
		newQuestInfo.State = iter->State;
		newQuestInfo.isDo = iter->isDo;
		newQuestInfo.isCompletion = iter->isCompletion;
		newQuestInfo.ArrayIndex = iter->ArrayIndex;
		newQuestInfo.isVisibles[0] = iter->isVisibles[0];
		newQuestInfo.isVisibles[1] = iter->isVisibles[1];
		newQuestInfo.isVisibles[2] = iter->isVisibles[2];
		newQuestInfo.isVisibles[3] = iter->isVisibles[3];
		newQuestInfo.isVisibles[4] = iter->isVisibles[4];
		newQuestInfo.KillEnemyList = iter->KillEnemyList;
		newQuestInfo.EnemyKillCount = iter->EnemyKillCount;
		newQuestInfo.RequiredItemsList = iter->RequiredItemsList;
		newQuestInfo.AssignedCharacters[0] = nullptr;
		newQuestInfo.AssignedCharacters[1] = nullptr;
		newQuestInfo.AssignedCharacters[2] = nullptr;
		newQuestInfo.AssignedCharacters[3] = nullptr;
		newQuestInfo.AssignedCharacters[4] = nullptr;
		newQuestInfo.isAutomationSet = iter->isAutomationSet;
		newQuestInfo.CompleteRewardItemsList.Empty();

		for (auto item : iter->CompleteRewardItemsList)
		{
			FSoftObjectPath retrieved_soft_object_path(item.Key->GetClass()->GetPathName());
			UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager pathname %s, path name %s"), *(item.Key->GetClass()->GetPathName()),
				*retrieved_soft_object_path.ToString());
			newQuestInfo.CompleteRewardItemsList.Add(retrieved_soft_object_path, item.Value);
		}

		newQuestInfo.CompleteRewardGold = iter->CompleteRewardGold;
		newQuestInfo.CompleteRewardExp = iter->CompleteRewardExp;
		newQuestInfo.CompleteSetState = iter->CompleteSetState;

		if (iter->GoalType == QuestGoalType::None)
		{
			goaltypearray.Add(0);
		}
		else if (iter->GoalType == QuestGoalType::Item_Required)
		{
			goaltypearray.Add(1);
		}
		else if (iter->GoalType == QuestGoalType::Kill_Enemy)
		{
			goaltypearray.Add(2);
		}
		else if (iter->GoalType == QuestGoalType::Reach_Position)
		{
			goaltypearray.Add(3);
		}

		switch (iter->QuestProgress)
		{
		case EQuestProgress::NONE:
			progressarray.Add(0);
			break;
		case EQuestProgress::NotExpose:
			progressarray.Add(1);
			break;
		case EQuestProgress::Ready:
			progressarray.Add(2);
			break;
		case EQuestProgress::OnGoing:
			progressarray.Add(3);
			break;
		case EQuestProgress::Complete:
			progressarray.Add(4);
			break;
		case EQuestProgress::End:
			progressarray.Add(5);
			break;
		default:
			progressarray.Add(0);
			break;
		}

		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager 1 :: Name - %s, Content - %s, state - %d,  QuestProgress - %s"),
			*(newQuestInfo.Name), *(newQuestInfo.Content), newQuestInfo.State, *GetEnumName(iter->QuestProgress));
		questarray.Add(newQuestInfo);
	}

	for (auto iter : questarray)
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager QuestArray :: Name - %s, Content - %s, state - %d"),
			*(iter.Name), *(iter.Content), iter.State);
	}

	for (auto iter : DoingQuestArray)
	{
		newQuestInfo.Name = iter->Name;
		newQuestInfo.Content = iter->Content;
		newQuestInfo.State = iter->State;
		newQuestInfo.isDo = iter->isDo;
		newQuestInfo.isCompletion = iter->isCompletion;
		newQuestInfo.ArrayIndex = iter->ArrayIndex;
		newQuestInfo.isVisibles[0] = iter->isVisibles[0];
		newQuestInfo.isVisibles[1] = iter->isVisibles[1];
		newQuestInfo.isVisibles[2] = iter->isVisibles[2];
		newQuestInfo.isVisibles[3] = iter->isVisibles[3];
		newQuestInfo.isVisibles[4] = iter->isVisibles[4];
		newQuestInfo.KillEnemyList = iter->KillEnemyList;
		newQuestInfo.EnemyKillCount = iter->EnemyKillCount;
		newQuestInfo.RequiredItemsList = iter->RequiredItemsList;
		newQuestInfo.AssignedCharacters[0] = nullptr;
		newQuestInfo.AssignedCharacters[1] = nullptr;
		newQuestInfo.AssignedCharacters[2] = nullptr;
		newQuestInfo.AssignedCharacters[3] = nullptr;
		newQuestInfo.AssignedCharacters[4] = nullptr;
		newQuestInfo.isAutomationSet = iter->isAutomationSet;
		newQuestInfo.CompleteRewardItemsList.Empty();

		for (auto item : iter->CompleteRewardItemsList)
		{
			FSoftObjectPath retrieved_soft_object_path(item.Key->GetClass()->GetPathName());

			newQuestInfo.CompleteRewardItemsList.Add(retrieved_soft_object_path, item.Value);
		}

		newQuestInfo.CompleteRewardGold = iter->CompleteRewardGold;
		newQuestInfo.CompleteRewardExp = iter->CompleteRewardExp;
		newQuestInfo.CompleteSetState = iter->CompleteSetState;
		
		doingquestarray.Add(newQuestInfo);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager 2 :: Name - %s, Content - %s, state - %d"),
			*(newQuestInfo.Name), *(newQuestInfo.Content), newQuestInfo.State);

		for (auto iter2 : newQuestInfo.KillEnemyList)
		{
			UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager 2 2 :: GoalType - %d, QuestInfo Enemylist : %d, Enemy Name : %s, Enemy count : %d"),
				iter->GoalType, newQuestInfo.KillEnemyList.Num(), *(iter2.Key), iter2.Value);
		}
	}

	for (auto iter : EndQuestArray)
	{
		newQuestInfo.Name = iter->Name;
		newQuestInfo.Content = iter->Content;
		newQuestInfo.State = iter->State;
		newQuestInfo.isDo = iter->isDo;
		newQuestInfo.isCompletion = iter->isCompletion;
		newQuestInfo.ArrayIndex = iter->ArrayIndex;
		newQuestInfo.isVisibles[0] = iter->isVisibles[0];
		newQuestInfo.isVisibles[1] = iter->isVisibles[1];
		newQuestInfo.isVisibles[2] = iter->isVisibles[2];
		newQuestInfo.isVisibles[3] = iter->isVisibles[3];
		newQuestInfo.isVisibles[4] = iter->isVisibles[4];
		newQuestInfo.KillEnemyList = iter->KillEnemyList;
		newQuestInfo.EnemyKillCount = iter->EnemyKillCount;
		newQuestInfo.RequiredItemsList = iter->RequiredItemsList;
		newQuestInfo.AssignedCharacters[0] = nullptr;
		newQuestInfo.AssignedCharacters[1] = nullptr;
		newQuestInfo.AssignedCharacters[2] = nullptr;
		newQuestInfo.AssignedCharacters[3] = nullptr;
		newQuestInfo.AssignedCharacters[4] = nullptr;
		newQuestInfo.isAutomationSet = iter->isAutomationSet;
		newQuestInfo.CompleteRewardItemsList.Empty();

		for (auto item : iter->CompleteRewardItemsList)
		{
			FSoftObjectPath retrieved_soft_object_path(item.Key->GetClass()->GetPathName());

			newQuestInfo.CompleteRewardItemsList.Add(retrieved_soft_object_path, item.Value);
		}

		newQuestInfo.CompleteRewardGold = iter->CompleteRewardGold;
		newQuestInfo.CompleteRewardExp = iter->CompleteRewardExp;
		newQuestInfo.CompleteSetState = iter->CompleteSetState;

		endquestarray.Add(newQuestInfo);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager 3 :: Name - %s, Content - %s, state - %d"),
			*(newQuestInfo.Name), *(newQuestInfo.Content), newQuestInfo.State);
	}

	for (auto iter : QuestHashTagArray)
	{
		hashtagarray.Add(iter);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: SaveQuestManager 4 :: Tag - %s"), *iter);
	}
}

void UQuestManager::LoadQuestManager(TArray<FQuestInfoSaveData>& questarray, TArray<FQuestInfoSaveData>& doingquestarray,
	TArray<FQuestInfoSaveData>& endquestarray, TArray<FString>& hashtagarray, TArray<int32>& goaltypearray,
	TArray<int32>& progressarray)
{
	UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager Num :: QuestArray - %d, DOing - %d, End - %d, hashtag - %d"),
		questarray.Num(), doingquestarray.Num(), endquestarray.Num(), hashtagarray.Num());

	//QuestArray.Empty();
	DoingQuestArray.Empty();
	EndQuestArray.Empty();
	QuestHashTagArray.Empty();

	UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 333 goaltuypearray size : %d"), sizeof(goaltypearray));
	UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 334 goaltuypearray num : %d"), goaltypearray.Num());


	for (int i = 0; i < questarray.Num(); i++)
	{
		UQuestInfo* newquestinfo;
		newquestinfo = QuestArray[i];

		newquestinfo->Name = questarray[i].Name;
		newquestinfo->Content = questarray[i].Content;
		newquestinfo->State = questarray[i].State;
		newquestinfo->isDo = questarray[i].isDo;
		newquestinfo->isCompletion = questarray[i].isCompletion;
		newquestinfo->ArrayIndex = questarray[i].ArrayIndex;
		newquestinfo->isVisibles[0] = questarray[i].isVisibles[0];
		newquestinfo->isVisibles[1] = questarray[i].isVisibles[1];
		newquestinfo->isVisibles[2] = questarray[i].isVisibles[2];
		newquestinfo->isVisibles[3] = questarray[i].isVisibles[3];
		newquestinfo->isVisibles[4] = questarray[i].isVisibles[4];
		newquestinfo->KillEnemyList = questarray[i].KillEnemyList;
		newquestinfo->EnemyKillCount = questarray[i].EnemyKillCount;
		newquestinfo->RequiredItemsList = questarray[i].RequiredItemsList;
		newquestinfo->AssignedCharacters[0] = nullptr;
		newquestinfo->AssignedCharacters[1] = nullptr;
		newquestinfo->AssignedCharacters[2] = nullptr;
		newquestinfo->AssignedCharacters[3] = nullptr;
		newquestinfo->AssignedCharacters[4] = nullptr;
		newquestinfo->isAutomationSet = questarray[i].isAutomationSet;
		newquestinfo->CompleteRewardItemsList.Empty();

		for (TTuple<FSoftObjectPath, int32> item_tuple : questarray[i].CompleteRewardItemsList)
		{
			UClass* LoadedClass = Cast<UClass>(item_tuple.Key.TryLoad());
			if (LoadedClass)
			{
				UItemBase* loaded_item = NewObject<UItemBase>(this, LoadedClass);

				//UItemBase* loaded_item = Cast<UItemBase>(StaticLoadObject(UItemBase::StaticClass(), nullptr, *item_tuple.Key.ToString()));

				UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager pathname %s"), *(item_tuple.Key.ToString()));

				newquestinfo->CompleteRewardItemsList.Add(loaded_item, item_tuple.Value);
				UE_LOG(LogTemp, Log, TEXT("UQuestManager Load QuestManager - 764 loaded_Item %d"), loaded_item);
				if (loaded_item)
				{
					UE_LOG(LogTemp, Log, TEXT("UQuestManager Load QuestManager - 766 loaded_Item %s"), *(loaded_item->Name.ToString()));
				}
			}
		}

		newquestinfo->CompleteRewardGold = questarray[i].CompleteRewardGold;
		newquestinfo->CompleteRewardExp = questarray[i].CompleteRewardExp;
		newquestinfo->CompleteSetState = questarray[i].CompleteSetState;

		if (goaltypearray[i] == 0)
		{
			newquestinfo->GoalType = QuestGoalType::None;
		}
		else if (goaltypearray[i] == 1)
		{
			newquestinfo->GoalType = QuestGoalType::Item_Required;
		}
		else if (goaltypearray[i] == 2)
		{
			newquestinfo->GoalType = QuestGoalType::Kill_Enemy;
		}
		else if (goaltypearray[i] == 3)
		{
			newquestinfo->GoalType = QuestGoalType::Reach_Position;
		}

		switch (progressarray[i])
		{
		case 0:
			newquestinfo->QuestProgress = EQuestProgress::NONE;
			break;
		case 1:
			newquestinfo->QuestProgress = EQuestProgress::NotExpose;
			break;
		case 2:
			newquestinfo->QuestProgress = EQuestProgress::Ready;
			break;
		case 3:
			newquestinfo->QuestProgress = EQuestProgress::OnGoing;
			break;
		case 4:
			newquestinfo->QuestProgress = EQuestProgress::Complete;
			break;
		case 5:
			newquestinfo->QuestProgress = EQuestProgress::End;
			break;
		default:
			newquestinfo->QuestProgress = EQuestProgress::NONE;
			break;
		}

		//QuestArray.Add(newquestinfo);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 1 questName - %s, QuestProgress - %s"), *(questarray[i].Name),
			*GetEnumName(newquestinfo->QuestProgress));
	}

	for (int j = 0; j < doingquestarray.Num(); j++)
	{
		for (int i = 0; i < QuestArray.Num(); i++)
		{
			if (QuestArray[i]->Name.Equals(doingquestarray[j].Name))
			{
				DoingQuestArray.Add(QuestArray[i]);

				UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 329 :: GoalType - %d"),
					QuestArray[i]->GoalType);

				if (QuestArray[i]->GoalType == QuestGoalType::Kill_Enemy)
				{
					UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 329 :: QuestInfo Enemylist : %d"), QuestArray[i]->KillEnemyList.Num());
					UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 329 :: Enemy Name : %s, Enemy count : %d")
						, *(QuestArray[i]->KillEnemyList.begin()->Key)
						, QuestArray[i]->KillEnemyList.begin()->Value);
				}
				//
				break;
			}
		}

		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 2 questName - %s"), *(doingquestarray[j].Name));
	}

	for (int j = 0; j < endquestarray.Num(); j++)
	{
		for (int i = 0; i < QuestArray.Num(); i++)
		{
			if (QuestArray[i]->Name.Equals(endquestarray[j].Name))
			{
				EndQuestArray.Add(QuestArray[i]);
				break;
			}
		}
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 3 questName - %s"), *(endquestarray[j].Name));
	}

	for (FString iter : hashtagarray)
	{
		QuestHashTagArray.Add(iter);
		UE_LOG(LogTemp, Log, TEXT("UQuestManager :: LoadQuestManager 4 questTag - %s"), *iter);
	}

	for (int i = 0; i < QuestArray.Num(); i++)
	{
		for (auto iter : QuestArray[i]->AssignedCharacters)
		{
			iter = nullptr;
		}
	}
}

void UQuestManager::InitQuestManager()
{
	DoingQuestArray.Empty();
	EndQuestArray.Empty();

	for (auto iter : QuestArray)
	{
		iter->State = 0;
		iter->isDo = false;
		iter->isCompletion = false;
		iter->ArrayIndex = -1;
		iter->AssignedCharacters.Empty();
		iter->AssignedCharacters.Add(nullptr);
		iter->AssignedCharacters.Add(nullptr);
		iter->AssignedCharacters.Add(nullptr);
		iter->AssignedCharacters.Add(nullptr);
		iter->AssignedCharacters.Add(nullptr);
		iter->QuestProgress = EQuestProgress::Ready;
		iter->isVisibles.Empty();
		iter->isVisibles.Add(true);
		iter->isVisibles.Add(false);
		iter->isVisibles.Add(false);
		iter->isVisibles.Add(false);
		iter->isVisibles.Add(false);

		iter->EnemyKillCount.Empty();
		iter->KillEnemyList.Empty();
		iter->RequiredItemsList.Empty();
		iter->GoalType = QuestGoalType::None;

		iter->isAutomationSet = false;
		iter->CompleteRewardItemsList.Empty();
		iter->CompleteRewardGold = 0;
		iter->CompleteRewardExp = 0;
		iter->CompleteSetState = -1;
	}
	UE_LOG(LogTemp, Log, TEXT("UQuestManger 488 ::  InitQuestManager execute"));
}