#include "BTTask_SelectSkill.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "G/CharacterInfoComponent.h"
#include "G/G.h"
#include "G/GCharacter.h"
#include "G/NPCAIController.h"
#include "G/Skill/SkillBase.h"
#include "G/Skill/SkillInfoManager.h"
#include "G/Skill/SkillSlot.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"

UBTTask_SelectSkill::UBTTask_SelectSkill()
{
	NodeName = TEXT("SelectSkill");
	for (int i = 0; i <= 10; i++)
	{
		_prioritySkillMap.Add(i, 0);
	}
}

EBTNodeResult::Type UBTTask_SelectSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AGCharacter* myCharacter = Cast<AGCharacter>(ControllingPawn);
	myCharacter->GetController()->StopMovement();

	int skillNum = myCharacter->GetSkillSystemComponent()->GetSkillSlotList().Num();

	TArray<int32> skillNumList = SelectGetSkillList(myCharacter);

	// 사용할 수 있는 스킬이 없으면 코스트 비워주기
	if (skillNumList.Num() == 0)
	{
		if (myCharacter->GetCharacterInfoManager()->GetECharacterName() == ECharacterName::NonPlayerCharacter)
		{
			ANPCAIController* myController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
			myController->SetCurCostKey(0);
		}
		return EBTNodeResult::Failed;
	}
	
	int randomNum = FMath::RandRange(0, skillNumList.Num() - 1);
	randomNum = skillNumList[randomNum];

	USkillSlot* mySkillSlot = myCharacter->GetSkillSystemComponent()->SetSelectedSkill(randomNum);
	// 잘못된 인덱스 접근 시 Fail
	if (!mySkillSlot)
	{
		return EBTNodeResult::Failed;
	}

	LOG(Log, "%s는 %s를 선택했다.", *myCharacter->GetCharacterInfoManager()->GetCharacterName().ToString(),
	    *mySkillSlot->GetMySkill()->GetSkillInfoManager()->GetSkillName().ToString());
	BlackboardComp->SetValueAsBool(ANPCAIController::BSelectSkillKey, true);

	return EBTNodeResult::Succeeded;


	return EBTNodeResult::Failed;
}


TArray<int32> UBTTask_SelectSkill::SelectGetSkillList(AGCharacter* myCharacter)
{
	int skillNum = myCharacter->GetSkillSystemComponent()->GetSkillSlotList().Num();
	TArray<int> skillNumList;
	int32 topPriority = -1;
	for (int i = 0; i < skillNum; i++)
	{
		USkillBase* mySkill = myCharacter->GetSkillSystemComponent()->GetSkillSlot(i)->GetMySkill();
		const int skillCost = mySkill->GetSkillInfoManager()->GetCost();
		const int coolDown = mySkill->GetSkillInfoManager()->GetCurCoolDown();

		if (IsRandom)
		{
			// 랜덤 스킬 선택
			if (mySkill->CheckSelectPossibility())
			{
				skillNumList.Add(i);
			}
		}
		else
		{
			// 우선순위 선택
			if (mySkill->CheckSelectPossibility() && _prioritySkillMap.Contains(i) && _prioritySkillMap[i] >=
				topPriority)
			{
				if (_prioritySkillMap[i] > topPriority)
				{
					topPriority = _prioritySkillMap[i];
					skillNumList.Empty();
				}
				skillNumList.Add(i);
			}
		}
	}
	return skillNumList;
}
