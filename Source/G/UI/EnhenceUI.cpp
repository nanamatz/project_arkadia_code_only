// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhenceUI.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "G/GGameInstance.h"
#include "G/Manager/InventoryManager.h"
#include "G/Item/WeaponItemBase.h"
#include "G/Manager/InGameDataManager.h"
#include "G/PlayerCharacter.h"
#include "G/Util/Define.h"
#include <Kismet/GameplayStatics.h>


void UEnhenceUI::NativeOnInitialized()
{
	UE_LOG(LogTemp, Log, TEXT("EnhenceUI(23) NativeOnInitialized Execute"));

	//상단 버튼 할당
	Button_FirstChar->OnClicked.AddDynamic(this, &UEnhenceUI::FirstCharButtonClicked);
	Button_SecondChar->OnClicked.AddDynamic(this, &UEnhenceUI::SecondCharButtonClicked);
	Button_ThirdChar->OnClicked.AddDynamic(this, &UEnhenceUI::ThirdCharButtonClicked);
	Button_FourthChar->OnClicked.AddDynamic(this, &UEnhenceUI::FourthCharButtonClicked);

	//하단 버튼 할당
	AcceptButton->OnClicked.AddDynamic(this, &UEnhenceUI::AcceptButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UEnhenceUI::CancelButtonClicked);

	//알림창 버튼 할당
	AlertCheckButton->OnClicked.AddDynamic(this, &UEnhenceUI::AlertCheckClicked);

	Button_SecondChar->SetVisibility(ESlateVisibility::Hidden);
	Button_ThirdChar->SetVisibility(ESlateVisibility::Hidden);
	Button_FourthChar->SetVisibility(ESlateVisibility::Hidden);

	AlertSizeBox->SetVisibility(ESlateVisibility::Collapsed);

}

void UEnhenceUI::init()
{
	UE_LOG(LogTemp, Log, TEXT("EnhenceUI(47) init Execute"));

	//상단 버튼 할당
	Button_FirstChar->OnClicked.AddDynamic(this, &UEnhenceUI::FirstCharButtonClicked);
	Button_SecondChar->OnClicked.AddDynamic(this, &UEnhenceUI::SecondCharButtonClicked);
	Button_ThirdChar->OnClicked.AddDynamic(this, &UEnhenceUI::ThirdCharButtonClicked);
	Button_FourthChar->OnClicked.AddDynamic(this, &UEnhenceUI::FourthCharButtonClicked);

	//하단 버튼 할당
	AcceptButton->OnClicked.AddDynamic(this, &UEnhenceUI::AcceptButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UEnhenceUI::CancelButtonClicked);

	//알림창 버튼 할당
	AlertCheckButton->OnClicked.AddDynamic(this, &UEnhenceUI::AlertCheckClicked);

	Button_SecondChar->SetVisibility(ESlateVisibility::Hidden);
	Button_ThirdChar->SetVisibility(ESlateVisibility::Hidden);
	Button_FourthChar->SetVisibility(ESlateVisibility::Hidden);

	AlertSizeBox->SetVisibility(ESlateVisibility::Collapsed);

	////버튼 이미지 셋팅
	//Button_FirstChar->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
	//Button_SecondChar->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
	//Button_ThirdChar->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
	//Button_FourthChar->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	//Button_FirstChar->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
	//Button_SecondChar->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
	//Button_ThirdChar->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
	//Button_FourthChar->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	//Button_FirstChar->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
	//Button_SecondChar->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
	//Button_ThirdChar->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
	//Button_FourthChar->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}

void UEnhenceUI::FindSelectCharEquip(FString EquipType)
{
	UE_LOG(LogTemp, Log, TEXT("EnhenceUI(71) FindSelectCharEquip Execute"));
	// 게임 인스턴스 저장
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//UItemBase* temp;

	//UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	Charlist = GameInstance->GetInGameDataManager()->GetPlayerCharacterList();

	if (Charlist.Num() == 0)
	{
		return;
	}

	SelectCharInfoSetUp();

	FCharacterInfo character_info = Charlist[SelectCharNumber]->GetCharacterInfo();

	//SelectItemInfo = *(character_info->equipped_item_map.Find(EEquipType::weapon));

	SelectItemInfo = nullptr;

	for (auto iter : character_info.equipped_item_map)
	{
		if (iter.Key == EEquipType::weapon)
		{
			SelectItemInfo = iter.Value;
		}
	}

	/*if (temp)
		return temp;
	else
		return nullptr;*/

	/*APlayerCharacter* character = Charlist[CharNum];
	ECharacterName charname = character->GetECharacterName();

	for (int32 i = 0; (temp = GameInstance->inventory->GetItemBaseAtIndex(i)) != nullptr; i++)
	{
		if (temp->GetItemType().Equals(EquipType))
		{
			if (charname == static_cast<UWeaponItemBase*>(temp)->GetEquipCharName())
			{
				return temp;
			}
		}
	}*/

	UE_LOG(LogTemp, Log, TEXT("EnhenceUI(110) FindSelectCharEquip Execute complete"));
}

void UEnhenceUI::FindSelectCharWeapon()
{
	FindSelectCharEquip("Weapon");
}

void UEnhenceUI::EquipInfoSetUp()
{
	if (SelectItemInfo != nullptr)
	{
		Equip_Image->SetBrushFromTexture(SelectItemInfo->GetIcon());
		Equip_Text->SetText(SelectItemInfo->Name);

		//일단은 무기만 강화한다고 가정
		int32 present_enhence_level = SelectItemInfo->GetEnhenceLevel();
		int32 expectation_enhence_level = present_enhence_level + 1;
		FString enhencetext = FString::Printf(TEXT("%d -> %d"), present_enhence_level, expectation_enhence_level);
		EnhenceLevelText->SetText(FText::FromString(enhencetext));
	}
	else
	{
		Equip_Image->SetBrushFromTexture(defaultImage);
		Equip_Text->SetText(FText::FromString("-"));
		EnhenceLevelText->SetText(FText::FromString("-"));
	}
}

void UEnhenceUI::AbilityInfoSetUp()
{
	AbilityScrollBox->ClearChildren();

	if (SelectItemInfo)
	{
		UTextBlock* increaseText;

		//텍스트 위젯 생성
		increaseText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

		//강화 종류에 따른 텍스트 생성 - 현재는 공격력만
		int32 present_power = static_cast<UWeaponItemBase*>(SelectItemInfo)->GetPower();
		int32 expectation_power = static_cast<UWeaponItemBase*>(SelectItemInfo)->IncreasePower;
		FString textstr = FString::Printf(TEXT("공격력    %d → %d"), present_power, present_power + expectation_power);

		//완성된 텍스트 설정 및 추가
		increaseText->SetText(FText::FromString(textstr));
		AbilityScrollBox->AddChild(increaseText);
	}
}

void UEnhenceUI::MaterialInfoSetUp()
{
	MaterialScrollBox->ClearChildren();

	SelectEquipCanEnhance = true;

	if (SelectItemInfo)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		int32 inventoryamount;
		UTextBlock* MaterialText;
		UItemBase* TempItemBase;

		for (auto Material : SelectItemInfo->EnhenceMaterial)
		{
			MaterialText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			TempItemBase = Material.Key.GetDefaultObject();
			inventoryamount = GameInstance->inventory->GetItemCount(TempItemBase->Name);

			if (inventoryamount < Material.Value)
			{
				SelectEquipCanEnhance = false;
			}

			FString textstr = FString::Printf(TEXT("%s    %d / %d"), *TempItemBase->Name.ToString(), inventoryamount,
			                                  Material.Value);

			MaterialText->SetText(FText::FromString(textstr));
			MaterialScrollBox->AddChild(MaterialText);
		}

		if (GameInstance->inventory->inventory.gold < SelectItemInfo->EnhenceGold)
			SelectEquipCanEnhance = false;
	}
}

void UEnhenceUI::SelectCharInfoSetUp()
{
	int charcount = Charlist.Num();
	UTexture2D* thumbnail_image;


	if (charcount > 0)
	{
		Button_FirstChar->SetVisibility(ESlateVisibility::Visible);
		//FVector2D size = FVector2D(Charlist[0]->GetCharacterInfo()->Thumbnail->GetSizeX(), Charlist[0]->GetCharacterInfo()->Thumbnail->GetSizeY());
		//Button_FirstChar->WidgetStyle.Normal.SetImageSize(size);
		thumbnail_image = Charlist[0]->GetCharacterInfoManager()->GetCharacterInfo().Thumbnail;
		Button_FirstChar->WidgetStyle.Normal.SetResourceObject(thumbnail_image);
		Button_FirstChar->WidgetStyle.Hovered.SetResourceObject(thumbnail_image);
		Button_FirstChar->WidgetStyle.Pressed.SetResourceObject(thumbnail_image);
	}

	if (charcount > 1)
	{
		Button_SecondChar->SetVisibility(ESlateVisibility::Visible);
		thumbnail_image = Charlist[1]->GetCharacterInfoManager()->GetCharacterInfo().Thumbnail;
		Button_SecondChar->WidgetStyle.Normal.SetResourceObject(thumbnail_image);
		Button_SecondChar->WidgetStyle.Hovered.SetResourceObject(thumbnail_image);
		Button_SecondChar->WidgetStyle.Pressed.SetResourceObject(thumbnail_image);
	}

	if (charcount > 2)
	{
		Button_ThirdChar->SetVisibility(ESlateVisibility::Visible);
		thumbnail_image = Charlist[2]->GetCharacterInfoManager()->GetCharacterInfo().Thumbnail;
		Button_ThirdChar->WidgetStyle.Normal.SetResourceObject(thumbnail_image);
		Button_ThirdChar->WidgetStyle.Hovered.SetResourceObject(thumbnail_image);
		Button_ThirdChar->WidgetStyle.Pressed.SetResourceObject(thumbnail_image);
	}

	if (charcount > 3)
	{
		Button_FourthChar->SetVisibility(ESlateVisibility::Visible);
		thumbnail_image = Charlist[3]->GetCharacterInfoManager()->GetCharacterInfo().Thumbnail;
		Button_FourthChar->WidgetStyle.Normal.SetResourceObject(thumbnail_image);
		Button_FourthChar->WidgetStyle.Hovered.SetResourceObject(thumbnail_image);
		Button_FourthChar->WidgetStyle.Pressed.SetResourceObject(thumbnail_image);
	}
}

//UI 정보 갱신
void UEnhenceUI::Update()
{
	//FindSelectCharWeapon();
	FindSelectCharEquip("Weapon");
	EquipInfoSetUp();
	AbilityInfoSetUp();
	MaterialInfoSetUp();
}

void UEnhenceUI::FirstCharButtonClicked()
{
	if (!IsAlert)
	{
		SelectCharNumber = 0;
		Update();
	}
}

void UEnhenceUI::SecondCharButtonClicked()
{
	if (!IsAlert)
	{
		SelectCharNumber = 1;
		Update();
	}
}

void UEnhenceUI::ThirdCharButtonClicked()
{
	if (!IsAlert)
	{
		SelectCharNumber = 2;
		Update();
	}
}

void UEnhenceUI::FourthCharButtonClicked()
{
	if (!IsAlert)
	{
		SelectCharNumber = 3;
		Update();
	}
}

//강화 시도
void UEnhenceUI::AcceptButtonClicked()
{
	if (!IsAlert)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UWeaponItemBase* base;
		if (SelectEquipCanEnhance && SelectItemInfo)
		{
			base = static_cast<UWeaponItemBase*>(SelectItemInfo);
			base->Enhence();
			for (auto Material : base->EnhenceMaterial)
			{
				//GameInstance->inventory->DeleteItemAmount(Material.Key.GetDefaultObject()->Name, Material.Value);
				GameInstance->inventory->DeleteItem(
					GameInstance->inventory->GetItemByName(Material.Key.GetDefaultObject()->Name), Material.Value);
			}

			GameInstance->inventory->AddGold(-(SelectItemInfo->EnhenceGold));

			AlertSizeBox->SetVisibility(ESlateVisibility::Visible);
			IsAlert = true;
			AlertText->SetText(FText::FromString(FString::Printf(TEXT("강화 성공!"))));
		}
		else
		{
			AlertText->SetText(NSLOCTEXT("EnhanceUI", "EnhanceFailed", "재료 부족"));
			AlertSizeBox->SetVisibility(ESlateVisibility::Visible);
			IsAlert = true;
		}
		Update();
	}
}

//UI 닫기
void UEnhenceUI::CancelButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UEnhenceUI(309) : Cancelbuttonclicked execute"));
	if (!IsAlert)
	{
		RemoveFromViewport();
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if(GameInstance)
			GameInstance->SetInputState(EInputState::Default);
	}
}

void UEnhenceUI::AlertCheckClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UEnhenceUI(318) : AlertCheckClicked execute"));
	AlertSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	IsAlert = false;
}
