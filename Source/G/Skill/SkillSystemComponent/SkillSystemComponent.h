// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "G/Skill/SkillData/SkillStruct.h"
#include "SkillSystemComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class G_API USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillSystemComponent();

	// 스킬 리스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class USkillSlot*> skillSlotList;

	// 고른 스킬 정보
	int selectedSkillNum;

	UPROPERTY()
	class USkillSlot* selectedSkillSlot;

	//공격 대상
	UPROPERTY()
	class AGCharacter* _skillTarget;

	//논타겟 공격 위치
	UPROPERTY()
	FVector _skillPos;

protected:
	UPROPERTY()
	TArray<class USkillBase*> _skillList;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void InitSkillSlot();

	void SetSkillSlot(class AGCharacter* myCharacter, class UDataTable* _skillData, bool isStart = true);

	void SetStartSkillSlot(class AGCharacter* myCharacter, class UDataTable* _skillData);
	void SetLoadDataSkillSlot(class AGCharacter* myCharacter);
	void SetChangeLevelSkillSlot(class AGCharacter* myCharacter, class UDataTable* _skillData);
	
	class USkillSlot* SetSelectedSkill(int SkillNum);
	void SetSkillTarget(class AGCharacter* skillTarget) { _skillTarget = skillTarget; }
	void SetSkillPos(FVector skillPos) { _skillPos = skillPos; }

	class TArray<class USkillBase*> GetSkillList();
	TArray<class USkillSlot*> GetSkillSlotList();
	TArray<class USkillSlot*>& GetSkillSlotListRef();
	class USkillSlot* GetSelectedSkillSlot() { return selectedSkillSlot; }
	class UTexture2D* GetSkillSlotThumbnail(class USkillSlot* _skillSlot);

	class USkillSlot* GetSkillSlot(int _idx)
	{
		if (skillSlotList.IsValidIndex(_idx))
		{
			return skillSlotList[_idx];
		}
		return nullptr;
	}

	bool GetSelectedSkillInfo(struct FSkillStruct& out);
	int GetSelectedSkillNum() { return selectedSkillNum; }
	class AGCharacter* GetSkillTarget() const { return _skillTarget; }
	FVector& GetSkillPos() { return _skillPos; }

	void AddSkillSlot(class USkillSlot* _newSlot);
	void RemoveSelectedSkillSlot();
	void RemoveSkillSlot(USkillSlot* SkillSlot);
	void UseSkill();
	void UseSkill(class USkillSlot* _skillSlot);
	void CountSkillCoolDown();

	bool CheckSkillPossibility();
	bool CheckTargetInAttackRange(class AGCharacter* target);
};
