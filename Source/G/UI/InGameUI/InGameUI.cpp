// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "G/GGamemode.h"
#include "G/GGameInstance.h"
#include "G/GCharacter.h"
#include "G/Manager/CombatManager.h"
#include "G/UI/TurnQueueUI/TurnQueueWidget.h"
#include "G/UI/Map/MapWidget.h"
#include "G/UI/InGameUI/SkillButton.h"
#include "G/UI/InGameUI/PlayerPortrait.h"
#include "G/Skill/SkillSlot.h"
#include "G/UI/InteractionCharacterInfoUI.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


#include "Sound/SoundBase.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "CircularPlayerPortrait.h"
#include "ESCMenuWidget.h"
#include "MenuToggleButton.h"
#include "SkillButtonTooltip.h"
#include "Components/TextBlock.h"
#include "G/G.h"
#include "G/Controller/PlayerCombatController.h"
#include "G/UI/IndexButton.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInGameUI::CreateTurnQueue()
{
	if (bp_turn_queue_widget)
	{
		turn_queue_widget = Cast<UTurnQueueWidget>(CreateWidget(this, bp_turn_queue_widget));
		turn_queue_widget->Init();
	}
}

void UInGameUI::OnClickedTurnEnd()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->GetCombatManager()->CheckCombatState(PlayerTurn))
	{
		GameInstance->GetCombatManager()->TurnEnd();
	}
}

void UInGameUI::EndTurn()
{
	skill_portrait->Off();
	OffSkillBox();
}

void UInGameUI::EndCombat()
{
	OffSkillBox();
	turn_queue_widget->ClearPortrait();
	skill_portrait->UpdatePortrait();
	
}

UMapWidget* UInGameUI::GetMinimap()
{
	return minimap;
}

UESCMenuWidget* UInGameUI::GetEscMenu()
{
	return esc_menu;
}


void UInGameUI::AddPortrait(APlayerCharacter* pc)
{
	if (!portrait_arr.Contains(pc->GetECharacterName()))
	{
		UPlayerPortrait* new_portrait = CreateWidget<UPlayerPortrait>(this, bp_player_portrait);
		new_portrait->Init(pc);
		portrait_arr.Add(pc->GetECharacterName(), new_portrait);

		UVerticalBoxSlot* vb_slot = portrait_box->AddChildToVerticalBox(new_portrait);
		vb_slot->SetSize(ESlateSizeRule::Fill);
		vb_slot->SetHorizontalAlignment(HAlign_Fill);
		vb_slot->SetVerticalAlignment(VAlign_Fill);

		UCanvasPanelSlot* cs = Cast<UCanvasPanelSlot>(portrait_box->Slot);

		cs->LayoutData.Anchors.Maximum.Y += 0.1f;
		cs->LayoutData.Offsets.Bottom = 0;


		cs->SetLayout(cs->LayoutData);
	}
	else
	{
		portrait_arr[pc->GetECharacterName()]->UpdateHP();
	}
}

void UInGameUI::DeletePortrait(ECharacterName name)
{
	portrait_arr[name]->RemoveFromParent();

	UCanvasPanelSlot* cs = Cast<UCanvasPanelSlot>(portrait_box->Slot);

	cs->LayoutData.Anchors.Maximum.Y -= 0.1f;
	cs->LayoutData.Offsets.Bottom = 0;

	cs->SetLayout(cs->LayoutData);

	portrait_arr.Remove(name);
}

void UInGameUI::DeletePortrait(UPlayerPortrait* pp)
{
	pp->RemoveFromParent();

	UCanvasPanelSlot* cs = Cast<UCanvasPanelSlot>(portrait_box->Slot);
	
	cs->LayoutData.Anchors.Maximum.Y -= 0.1f;
	cs->LayoutData.Offsets.Bottom = 0;

	cs->SetLayout(cs->LayoutData);
}


void UInGameUI::ClearPortrait()
{
	for (auto portrait : portrait_arr)
	{
		DeletePortrait(portrait.Value);
	}

	portrait_arr.Empty();

	turn_queue_widget->ClearPortrait();
}

/*
    player_index portrait info update
*/
void UInGameUI::UpdatePortraitHP(ECharacterName name)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerCharacter* character = GameInstance->GetInGameDataManager()->GetPlayerCharacter(name);
	portrait_arr[name]->UpdateHP();
}

void UInGameUI::UpdateHP(ECharacterName name)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerCharacter* character = GameInstance->GetInGameDataManager()->GetPlayerCharacter(name);

	portrait_arr[name]->UpdateHP();
	turn_queue_widget->UpdateHP(character);
	skill_portrait->UpdatePortrait();
}

UTurnQueueWidget* UInGameUI::GetTurnQueueUI()
{
	return turn_queue_widget;
}

UHorizontalBox* UInGameUI::GetSkillCost()
{
	return skill_cost;
}

UCircularPlayerPortrait* UInGameUI::GetCircularPortrait()
{
	return skill_portrait;
}

void UInGameUI::SetCost(AGCharacter* character)
{
	int32 max_cost = character->GetMaxCost();
	curr_cost = max_cost;
	int32 curr_skill_cost = skill_cost->GetChildrenCount();

	if (curr_skill_cost < max_cost)
	{
		UE_LOG(LogTemp, Log, TEXT("SetCost: curr_skill_cost < max_cost"));
		AddCostBar(max_cost - curr_skill_cost);
	}
	else if (curr_skill_cost > max_cost)
	{
		UE_LOG(LogTemp, Log, TEXT("SetCost: curr_skill_cost > max_cost"));
		RemoveCostBar(curr_skill_cost - max_cost);
	}

	UpdateCost(character);
}

void UInGameUI::UpdateCost(AGCharacter* character)
{
	int32 max_cost = character->GetMaxCost();
	int32 cur_cost = character->GetCurrentCost();
	curr_cost = cur_cost;

	for (int i = 0; i < max_cost; i++)
	{
		if (i < cur_cost)
		{
			UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(i));
			cost_img->SetColorAndOpacity(CostColor);
		}
		else
		{
			UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(i));
			cost_img->SetColorAndOpacity(UsedColor);
		}
	}
}

void UInGameUI::CreateInteractCharInfoUI()
{
	if (bp_interact_char_info_ui)
	{
		interact_char_info_ui = Cast<UInteractionCharacterInfoUI>(CreateWidget(this, bp_interact_char_info_ui));
		interact_char_info_ui->DeActive();
	}
}

USkillButton* UInGameUI::AddSkillButton(int32 skill_num)
{
	USkillButton* new_button = CreateWidget<USkillButton>(this, bp_skill_button);

	new_button->Init(skill_button_tooltip, skill_num);

	button_array.Add(new_button);

	UHorizontalBoxSlot* horizontal_box_slot = skill_box->AddChildToHorizontalBox(new_button);

	horizontal_box_slot->SetSize(ESlateSizeRule::Fill);
	horizontal_box_slot->SetVerticalAlignment(VAlign_Center);
	horizontal_box_slot->SetHorizontalAlignment(HAlign_Center);
	horizontal_box_slot->SetPadding(0);

	return new_button;
}

void UInGameUI::AddSkillButton(int32 first_skill_num, int32 cnt)
{
	for(int i = 0; i < cnt; i++)
	{
		USkillButton* new_button = CreateWidget<USkillButton>(this, bp_skill_button);

		new_button->Init(skill_button_tooltip, first_skill_num + i);

		button_array.Add(new_button);

		UHorizontalBoxSlot* horizontal_box_slot = skill_box->AddChildToHorizontalBox(new_button);

		horizontal_box_slot->SetSize(ESlateSizeRule::Fill);
		horizontal_box_slot->SetVerticalAlignment(VAlign_Center);
		horizontal_box_slot->SetHorizontalAlignment(HAlign_Center);
		horizontal_box_slot->SetPadding(0);
	}
}

bool UInGameUI::Initialize()
{
	bool res = Super::Initialize();

	if(inventory_bt)
	{
		inventory_bt->SetIndex(1);
	}

	if(skill_bt)
	{
		skill_bt->SetIndex(2);
	}

	if(quest_bt)
	{
		quest_bt->SetIndex(3);
	}

	if(map_bt)
	{
		map_bt->SetIndex(4);
	}

	if(setting_bt)
	{
		setting_bt->OnClicked.AddDynamic(GetEscMenu(), &UESCMenuWidget::On);
	}

	if(skill_cancel_bt)
	{
		skill_cancel_bt->OnClicked.AddDynamic(this, &UInGameUI::CancelSkill);
	}

	if(plus_bt)
	{
		plus_bt->OnClicked.AddDynamic(this, &UInGameUI::UInGameUI::IncreaseGameSpeed);
	}

	if(minus_bt)
	{
		minus_bt->OnClicked.AddDynamic(this, &UInGameUI::UInGameUI::DecreaseGameSpeed);
	}
	
	return res;
}

void UInGameUI::IncreaseMaxCost(AGCharacter* character, int32 increasing_cost)
{
	AddCostBar(increasing_cost);
	UpdateCost(character);
}

void UInGameUI::DecreaseMaxCost(AGCharacter* character, int32 decreasing_cost)
{
	RemoveCostBar(decreasing_cost);
	UpdateCost(character);
}

void UInGameUI::AddCostBar(int32 value)
{
	for (int i = 0; i < value; i++)
	{
		UImage* new_img = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		new_img->SetBrushFromTexture(img_cost_bar);
		new_img->SetColorAndOpacity(CostColor);
		UHorizontalBoxSlot* hb_slot = skill_cost->AddChildToHorizontalBox(new_img);
		hb_slot->SetSize(ESlateSizeRule::Automatic);
		hb_slot->SetPadding(FMargin(5));
	}
}

void UInGameUI::RemoveCostBar(int32 value)
{
	for (int i = 0; i < value; i++)
	{
		UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(skill_cost->GetChildrenCount() - 1));
		cost_img->RemoveFromParent();
	}
}

void UInGameUI::UseCostBar(int32 value)
{
	for (int i = 0; i < value; i++)
	{
		UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(skill_cost->GetChildrenCount() - 1));
		cost_img->SetColorAndOpacity(UsedColor);
	}
}

void UInGameUI::HighlightCostReduction(int32 reduction)
{
	if (curr_cost >= reduction)
	{
		for (int i = 0; i < reduction; i++)
		{
			UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(curr_cost - i - 1));
			cost_img->SetColorAndOpacity(HighlightColor);
		}
	}
	else
	{
		for (int i = 0; i < curr_cost; i++)
		{
			UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(i));
			cost_img->SetColorAndOpacity(CostColor);
		}
	}
}

void UInGameUI::UnHighlightCostReduction()
{
	// int32 cost = skill_cost->GetChildrenCount();
	for (int i = 0; i < curr_cost; i++)
	{
		UImage* cost_img = Cast<UImage>(skill_cost->GetChildAt(i));
		cost_img->SetColorAndOpacity(CostColor);
	}
}

/*void UInGameUI::SetSkillButton(USkillSlot* _skillSlot, int index)
{
	USkillButton* target_button = button_array[index - 1];

	if (_skillSlot && target_button)
	{
		target_button->Set(_skillSlot);

		/* Set Skill Icon #1#
		target_button->skill_img->SetBrushFromTexture(_skillSlot->GetThumnail());
		target_button->skill_img->SetOpacity(1);
	}
}*/

void UInGameUI::SetSkillButton(TArray<USkillSlot*> SkillSlotList, bool isPlayerTurn)
{
	int button_arr_size = button_array.Num();
	int skill_slot_list_size = SkillSlotList.Num();

	if(button_arr_size < skill_slot_list_size)
	{
		AddSkillButton(button_arr_size, skill_slot_list_size - button_arr_size);
	}
	if(button_arr_size > skill_slot_list_size)
	{
		button_array.RemoveAt(skill_slot_list_size, button_arr_size - skill_slot_list_size);

		for(int i = button_arr_size; i > skill_slot_list_size; i--)
		{
			skill_box->RemoveChildAt(i-1);
		}
	}
	
	for (int i = 0; i < SkillSlotList.Num(); i++)
	{
		button_array[i]->Set(SkillSlotList[i], isPlayerTurn);
	}
}

void UInGameUI::SetSkillIcon(UTexture2D* icon, int index)
{
	button_array[index - 1]->skill_img->SetBrushFromTexture(icon);
	button_array[index - 1]->skill_img->SetOpacity(1);
}

void UInGameUI::Init()
{
	skill_button_tooltip = CreateWidget<USkillButtonTooltip>(this, bp_skill_button_tooltip);
	skill_button_tooltip->bIsFocusable = false;

	/*for (int32 i = 0; i < 10; i++)
		AddSkillButton(i);*/
	
	turn_end_button->OnClicked.AddDynamic(this, &UInGameUI::OnClickedTurnEnd);

	OffSkillBox();

	CreateTurnQueue();
	CreateInteractCharInfoUI();

	skill_portrait->Init();
	minimap->Init();
}

void UInGameUI::On()
{
	AddToViewport();
}

void UInGameUI::Off()
{
	RemoveFromViewport();
}

void UInGameUI::ActivePortrait(ECharacterName name)
{
	portrait_arr[name]->SetVisibility(ESlateVisibility::Visible);
}

void UInGameUI::DeactivePortrait(ECharacterName name)
{
	portrait_arr[name]->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameUI::StartPlayerTurn(APlayerCharacter* pc)
{
	turn_queue_widget->AnnouncementPlayerTurn();
	SetSkillPortrait(pc);
}

void UInGameUI::EndPlayerTurn()
{
	skill_portrait->Off();
}

void UInGameUI::OnSkillContainer() const
{
	skill_container->SetVisibility(ESlateVisibility::Visible);
}

void UInGameUI::OffSkillContainer() const
{
	skill_container->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameUI::OnSkillBox() const
{
	skill_box->SetVisibility(ESlateVisibility::Visible);
}

void UInGameUI::OffSkillBox() const
{
	skill_box->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameUI::OnCost() const
{
	skill_cost->SetVisibility(ESlateVisibility::Visible);
}

void UInGameUI::OffCost() const
{
	skill_cost->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameUI::SetSkillPortrait(APlayerCharacter* pc)
{
	skill_portrait->SetPortrait(pc);
}

void UInGameUI::UpdateSkillPortrait()
{
	skill_portrait->UpdatePortrait();
}

void UInGameUI::UpdateControlCharacter() const
{
	minimap->UpdateControlCharacter();
}

void UInGameUI::OnInteractCharInfoUI(FString name, FVector2D pos, FVector2D offset)
{
	interact_char_info_ui->Init(name, pos, offset);
	interact_char_info_ui->Active();
}

void UInGameUI::OffInteractCharInfoUI()
{
	interact_char_info_ui->DeActive();
}

void UInGameUI::CancelSkill()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->GetPlayerCombatController()->SetPlayerInputState(ClickSkill);
}

void UInGameUI::IncreaseGameSpeed()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	int32 game_speed = GameInstance->IncreaseGameSpeed();

	game_speed_text->SetText(FText::Format(FText::FromString("x{0}"), game_speed));
}

void UInGameUI::DecreaseGameSpeed()
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	int32 game_speed = GameInstance->DecreaseGameSpeed();

	game_speed_text->SetText(FText::Format(FText::FromString("x{0}"), game_speed));
}

#undef LOCTEXT_NAMESPACE