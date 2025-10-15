// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Manager/ManagerBase.h"
#include "FQuestManager.generated.h"

UENUM()
enum class QuestGoalType
{
	None UMETA(DisplayName = "해당 없음"),
	Item_Required UMETA(DisplayName = "아이템 요구"),
	Kill_Enemy UMETA(DisplayName = "적 처치"),
	Reach_Position UMETA(DisplayName = "위치 도달")
};

UENUM()
enum class EQuestProgress : uint8
{
	NONE, //기본값
	NotExpose, //선행 조건이 완료되지 않음. 노출되지 않은 상태
	Ready, //퀘스트가 노출됨. 수락 대기
	OnGoing, //퀘스트 진행 중
	Complete, //퀘스트 완료, 완료 대기 중
	End //완료 후 완전히 종료
};

template <typename EnumType>
static FORCEINLINE FString GetEnumName(const EnumType InValue)
{
	return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InValue));
}

UCLASS()
class UQuestInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	FString Content;
	UPROPERTY(EditAnywhere)
	int32 State;
	UPROPERTY(EditAnywhere)
	bool isDo;
	UPROPERTY(EditAnywhere)
	bool isCompletion;
	UPROPERTY(EditAnywhere)
	int32 ArrayIndex;
	UPROPERTY(EditAnywhere)
	TArray<bool> isVisibles;

	UPROPERTY()
	QuestGoalType GoalType;

	UPROPERTY()
	TMap<FString, int32> KillEnemyList;

	UPROPERTY()
	TArray<int32> EnemyKillCount;

	UPROPERTY()
	TMap<FString, int32> RequiredItemsList;

	UPROPERTY()
	TArray<class AInteractionCharacter*> AssignedCharacters;

	UPROPERTY()
	EQuestProgress QuestProgress;

	UPROPERTY()
	bool isAutomationSet;

	UPROPERTY()
	TMap<class UItemBase*, int32> CompleteRewardItemsList;

	UPROPERTY()
	int32 CompleteRewardGold;

	UPROPERTY()
	int32 CompleteRewardExp;

	UPROPERTY()
	int32 CompleteSetState;

	UQuestInfo()
	{
		Name = FString("");
		Content = FString("");
		State = 0;
		isDo = false;
		isCompletion = false;
		ArrayIndex = -1;
		AssignedCharacters.Empty();
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		QuestProgress = EQuestProgress::Ready;
		isVisibles.Empty();
		isVisibles.Add(true);
		isVisibles.Add(false);
		isVisibles.Add(false);
		isVisibles.Add(false);
		isVisibles.Add(false);
		EnemyKillCount.Empty();
		KillEnemyList.Empty();
		RequiredItemsList.Empty();
		GoalType = QuestGoalType::None;
		isAutomationSet = false;
		CompleteRewardGold = 0;
		CompleteRewardExp = 0;
		CompleteSetState = -1;
	};
	~UQuestInfo()
	{
		UE_LOG(LogTemp, Log, TEXT("UQuestInfo destroy :: pointer is %d"), this);
	}
	
	void init(FString name, FString content)
	{
		Name = name;
		Content = content;
		State = 0;
		isDo = false;
		isCompletion = false;
		ArrayIndex = -1;
		AssignedCharacters.Empty();
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		AssignedCharacters.Add(nullptr);
		QuestProgress = EQuestProgress::Ready;
		isVisibles.Empty();
		isVisibles.Add(true);
		isVisibles.Add(false);
		isVisibles.Add(false);
		isVisibles.Add(false);
		isVisibles.Add(false);
		QuestInitialize();
	}

	void QuestInitialize()
	{
		EnemyKillCount.Empty();
		KillEnemyList.Empty();
		RequiredItemsList.Empty();
		GoalType = QuestGoalType::None;
		isAutomationSet = false;
		CompleteRewardItemsList.Empty();
		CompleteRewardGold = 0;
		CompleteRewardExp = 0;
		CompleteSetState = -1;
	}
};

UCLASS()
class G_API UQuestManager : public UManagerBase
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UQuestInfo*> QuestArray;
	UPROPERTY()
	TArray<UQuestInfo*> DoingQuestArray;
	UPROPERTY()
	TArray<UQuestInfo*> EndQuestArray;
	UPROPERTY()
	TArray<FString> QuestHashTagArray;

	//class UGGameInstance* GameInstance = nullptr;
	UPROPERTY()
	class UDataTable* QuestDataTable;
	
public:
	UQuestManager();

	~UQuestManager();

	virtual void Init(UGGameInstance* gameInstance) override;
	void GenerateQuestInfo(UGGameInstance*);
	int32 GetHashTag(FString str);

	FString GetName(int32 index) { return QuestArray[index]->Name; }
	FString GetContent(int32 index) { return QuestArray[index]->Content; }
	bool isDo(int32 index) { return QuestArray[index]->isDo; }
	bool isComplete(int32 index) { return QuestArray[index]->isCompletion; }

	void SetState(int32 index, int32 state) { QuestArray[index]->State = state; }
	int32 GetState(int32 index) { return QuestArray[index]->State; }

	void SetQuestProgress(int32 index, EQuestProgress qs) { QuestArray[index]->QuestProgress = qs; }
	EQuestProgress GetQuestProgress(int32 index) { return QuestArray[index]->QuestProgress; }

	void QuestDo(int32 index);
	void QuestComplete(int32 index);

	void QuestAcceptAnnounce(int32 index);
	void QuestAchieveAnnounce(int32 index);
	void QuestReceiptAnnounce(int32 index);

	TArray<UQuestInfo*> GetQuestList() { return QuestArray; }
	TArray<UQuestInfo*> GetDoingQuestList() { return DoingQuestArray; }
	TArray<UQuestInfo*> GetEndQuestList() { return EndQuestArray; }

	void QuestGoalInitialize(int32 index);
	void SetQuestGoalRequiredItem(int32 index, TMap<class UItemBase*, int32>);
	void SetQuestGoalKillEnemy(int32 index, TMap<class ANonPlayerCharacter*, int32>);
	void SetQuestGoalReachPosition(int32 index);
	bool CheckQuestGoalComplete(int32 index);
	void AddGoalKillCount(FString targetname);
	void ChangeGoalItemCount(class UItemBase* targetitem);

	void AchieveReachPositionIndex(int32 index);
	void AchieveReachPositionHashTag(FString tag);

	void SetQuestAutomationComplete(int32 index, TMap<class UItemBase*, int32> RewardItemsList, int32 RewardGold, int32 RewardExp, int32 RewardState);
	bool GetQuestAutomationSet(int32 index) { return QuestArray[index]->isAutomationSet; }
	void QuestRewardPayment(int32 index);

	//Inventory Clear Response Function
	void AllItemGoalQuestSetOnGoing();

	//Call when saving the game
	void SaveQuestManager(
		TArray<struct FQuestInfoSaveData> &quest_array,
		TArray<FQuestInfoSaveData> &doing_quest_array,
		TArray<FQuestInfoSaveData> &end_quest_array,
		TArray<FString> &hashtag_array,
		TArray<int32> &goaltypearray,
		TArray<int32> &progressarray);

	//Call when loading the game
	void LoadQuestManager(
		TArray<FQuestInfoSaveData> &quest_array,
		TArray<FQuestInfoSaveData> &doing_quest_array,
		TArray<FQuestInfoSaveData> &end_quest_array,
		TArray<FString> &hashtag_array,
		TArray<int32> &goaltypearray,
		TArray<int32>& progressarray);

	void InitQuestManager();
};
