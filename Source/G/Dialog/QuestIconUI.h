// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestIconUI.generated.h"

UENUM()
enum class QuestIconState : uint8
{
	NONE = 0, //�⺻��
	ReadyIcon, //����Ʈ ���� ���
	DoingIcon, //����Ʈ ���� ��
	CompleteIcon, //����Ʈ �Ϸ� ��� ��
};

/**
 * 
 */
UCLASS()
class G_API UQuestIconUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ImageIcon;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* QuestReadyImage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* QuestDoingImage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* QuestCompleteImage;

	bool visible = false;

public:
	void UpdateQuestIcon(QuestIconState newstate);
	void UpdateVisible(bool rrr);
};
