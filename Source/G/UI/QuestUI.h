// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMenuBase.h"
#include "Components/CanvasPanel.h"
#include "QuestUI.generated.h"

/**
 * 
 */

UCLASS()
class G_API UQuestUI : public UGameMenuBase
{
	GENERATED_BODY()

	/* variable */
	UPROPERTY(meta = (BindWidget))
	class UButton* active_button;

	UPROPERTY(meta = (BindWidget))
	class UButton* archived_button;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* quest_detail;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* active_quest_list;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* archived_quest_list;

	UPROPERTY(meta = (BindWidget))
	class UBorder* active_quest_list_border;

	UPROPERTY(meta = (BindWidget))
	class UBorder* archived_quest_list_border;

	/* for test */
	//UPROPERTY(meta = (BindWidget))
	//class UButton* add_quest;

	UPROPERTY(EditAnywhere, Category = "QuestContent")
	class UTexture2D* quest_default_img;

	UPROPERTY(EditAnywhere, Category = "QuestContent")
	struct FSlateFontInfo default_font;

	float text_size_x = 400.0f;
	float text_size_y = 40.0f;

protected:
	// virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	/* function */
private:

public:
	virtual void On() override;
	virtual void Off() override;

	void Init();
	void Update();

	UFUNCTION()
	void ActiveButton();

	UFUNCTION()
	void ArchivedButton();

	void Add_ActiveQuest(class UQuestInfo& quest);
	void Add_ActiveQuest(FString title, FString content);
	void Add_ArchivedQuest(UQuestInfo& quest);
	void Set_QuestDetail(FString text);
	void Remove_ActiveQuest(class UQuestContent* quest);
	void RemoveAt_ActiveQuest(int32 quest_index);

	void ClearUI();
	void Reconstitution();
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadDelegate, FString, name);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClickDelegate);

UCLASS()
class G_API UQuestContent : public UCanvasPanel
{
	GENERATED_BODY()

	/* variable */

	UPROPERTY()
	class UQuestUI* quest_ui;

	int32 list_text_size = 24;

public:
	UPROPERTY()
	class UTextBlock* list_text;

	UPROPERTY()
	class UImage* img;

	UPROPERTY()
	class UButton* button;

	FString detail_text;

	/* function */
	void Init(class UQuestUI* _quest_ui, FString Title, FString detail);

	UFUNCTION()
	void OnClick();
};
