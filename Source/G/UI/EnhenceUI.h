// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhenceUI.generated.h"

/**
 *
 */
UCLASS()
class G_API UEnhenceUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	class UTexture2D* defaultImage;

	int32 SelectCharNumber = 0; //선택 캐릭터 넘버

	UPROPERTY()
	TArray<class APlayerCharacter*> Charlist; //캐릭터 리스트

	UPROPERTY()
	class UEquipItemBase* SelectItemInfo = nullptr; //현재 보고 있는 아이템

	bool SelectEquipCanEnhance = true; //강화가 가능한지?
	bool IsAlert = false;

	//상단 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_FirstChar;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_SecondChar;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_ThirdChar;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_FourthChar;

	//장비 세부사항(UI좌측)
	UPROPERTY(meta = (BindWidget))
	class UImage* Equip_Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Equip_Text;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnhenceLevelText;

	//강화 세부사항 (UI 우측)
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* AbilityScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* MaterialScrollBox;

	//하단 상호작용 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* AcceptButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* AlertSizeBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* AlertCheckButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AlertText;

	//선택한 장비 찾는 함수 일반형
	void FindSelectCharEquip(FString EquipType);
	//무기 찾기 특화
	void FindSelectCharWeapon();

	//좌측 장비 정보 UI 갱신
	void EquipInfoSetUp();
	//우측 능력치 상승 UI 갱신
	void AbilityInfoSetUp();
	//우측 필요 재료 UI 갱신
	void MaterialInfoSetUp();

	virtual void NativeOnInitialized() override;
	//virtual void NativeConstruct() override;

public:
	//초기화
	void init();

	//UI의 정보를 갱신
	void Update();

	//상단 캐릭터 버튼 UI 갱신
	void SelectCharInfoSetUp();

	//캐릭터 전환 버튼 상호작용
	UFUNCTION()
	void FirstCharButtonClicked();

	UFUNCTION()
	void SecondCharButtonClicked();

	UFUNCTION()
	void ThirdCharButtonClicked();

	UFUNCTION()
	void FourthCharButtonClicked();

	//하단 버튼 상호작용
	//강화 시도
	UFUNCTION()
	void AcceptButtonClicked();

	//UI 닫기
	UFUNCTION()
	void CancelButtonClicked();

	//알림창 닫기
	UFUNCTION()
	void AlertCheckClicked();
};
