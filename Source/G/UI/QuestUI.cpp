// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/QuestUI.h"

#include <G/Manager/TutorialManager.h>

#include "GameMenuManager.h"
#include "G/GPlayerController.h"
#include "G/GGameInstance.h"
#include "G/GPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Blueprint/WidgetTree.h"
#include "G/G.h"
#include "G/Dialog/FQuestManager.h"

//#include "UObject/ScriptDelegates.h"

FReply UQuestUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey key = InKeyEvent.GetKey();

	if (key == EKeys::Q || key == EKeys::Escape)
	{
		UGameMenuManager* gm = Cast<UGameMenuManager>(GetOuter());

		if(gm)
		{
			LOG(Log, "gm valid");
			gm->Off();
		} else
		{
			UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->GetGameMenuManager()->Off();			
		}

		return FReply::Handled();
	}
	if (key == EKeys::I || key == EKeys::K || key == EKeys::M)
	{
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		GameInstance->GetGameMenuManager()->ChangeSubUI(key);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

/*
private
void UQuestUI::Set_QuestDetail()
{
	AGPlayerController* PlayerControllerInstance = GetWorld()->GetFirstPlayerController<AGPlayerController>();

	UButton* clicked_object = Cast<UButton>(PlayerControllerInstance->TraceUnderMouse());

	if (clicked_object) {
		int32 linker_index = clicked_object->GetLinkerIndex();
	}
}
*/
void UQuestUI::On()
{
	Super::On();
	this->Set_QuestDetail(FString(""));

	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UQuestUI::Off()
{
	Super::Off();
}

void UQuestUI::Init()
{
	UE_LOG(LogTemp, Log, TEXT("Quest Init"));
	active_button->OnClicked.AddDynamic(this, &UQuestUI::ActiveButton);
	archived_button->OnClicked.AddDynamic(this, &UQuestUI::ArchivedButton);
}

void UQuestUI::Update()
{
}

void UQuestUI::Add_ActiveQuest(UQuestInfo& questinfo)
{
	UQuestContent* new_quest = WidgetTree->ConstructWidget<UQuestContent>(UQuestContent::StaticClass());
	//UCanvasPanel* new_panel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());

	/* Becaus of WidgetTree, new_quest members are initialized in here */
	new_quest->list_text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	new_quest->img = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	new_quest->button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

	new_quest->AddChild(new_quest->img);
	new_quest->AddChild(new_quest->list_text);
	new_quest->AddChild(new_quest->button);

	/* set the button background color */
	new_quest->button->SetBackgroundColor(FLinearColor(1, 1, 1, 0));

	new_quest->img->SetBrushFromTexture(quest_default_img);

	/* set img anchors */
	UCanvasPanelSlot* canvas_slot = Cast<UCanvasPanelSlot>(new_quest->img->Slot);
	canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	canvas_slot->SetOffsets(FMargin(0));

	/* set button anchors */
	canvas_slot = Cast<UCanvasPanelSlot>(new_quest->button->Slot);
	canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	canvas_slot->SetOffsets(FMargin(0));

	/* set text anchors */
	canvas_slot = Cast<UCanvasPanelSlot>(new_quest->list_text->Slot);
	canvas_slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	canvas_slot->SetPosition(FVector2D(-(text_size_x / 2), -(text_size_y / 2)));
	canvas_slot->SetSize(FVector2D(text_size_x, text_size_y));

	FButtonStyle button_style;

	//slate_brush.
	button_style.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
	button_style.Normal.DrawAs = ESlateBrushDrawType::Image;

	button_style.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
	button_style.Pressed.DrawAs = ESlateBrushDrawType::Image;

	button_style.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
	button_style.Hovered.DrawAs = ESlateBrushDrawType::Image;

	new_quest->button->SetStyle(button_style);
	new_quest->Init(this, questinfo.Name, questinfo.Content);

	/* new_text : quest detail text */
	/* new_img : quest simbol img */
	/* new_button : make the quest-list clickable */

	active_quest_list->AddChild(new_quest);
}

void UQuestUI::Add_ActiveQuest(FString title, FString content)
{
	UQuestContent* new_quest = WidgetTree->ConstructWidget<UQuestContent>(UQuestContent::StaticClass());

	/* Becaus of WidgetTree, new_quest members are initialized in here */
	new_quest->list_text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	new_quest->img = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	new_quest->button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

	new_quest->list_text->SetFont(default_font);

	new_quest->AddChild(new_quest->img);
	new_quest->AddChild(new_quest->list_text);
	new_quest->AddChild(new_quest->button);

	/* set the button background color */
	new_quest->button->SetBackgroundColor(FLinearColor(1, 1, 1, 0));

	new_quest->img->SetBrushFromTexture(quest_default_img);

	/* set img anchors */
	UCanvasPanelSlot* canvas_slot = Cast<UCanvasPanelSlot>(new_quest->img->Slot);
	canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	canvas_slot->SetOffsets(FMargin(0));

	/* set button anchors */
	canvas_slot = Cast<UCanvasPanelSlot>(new_quest->button->Slot);
	canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
	canvas_slot->SetOffsets(FMargin(0));

	/* set text anchors */
	canvas_slot = Cast<UCanvasPanelSlot>(new_quest->list_text->Slot);
	canvas_slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	canvas_slot->SetPosition(FVector2D(-(text_size_x / 2), -(text_size_y / 2)));
	canvas_slot->SetSize(FVector2D(text_size_x, text_size_y));

	FButtonStyle button_style;

	//slate_brush.
	button_style.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
	button_style.Normal.DrawAs = ESlateBrushDrawType::Image;

	button_style.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
	button_style.Pressed.DrawAs = ESlateBrushDrawType::Image;

	button_style.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
	button_style.Hovered.DrawAs = ESlateBrushDrawType::Image;

	new_quest->button->SetStyle(button_style);
	new_quest->Init(this, title, content);

	/* new_text : quest detail text */
	/* new_img : quest simbol img */
	/* new_button : make the quest-list clickable */

	active_quest_list->AddChild(new_quest);
}

void UQuestUI::ActiveButton()
{
	archived_quest_list_border->SetVisibility(ESlateVisibility::Collapsed);
	archived_quest_list->SetVisibility(ESlateVisibility::Collapsed);
	active_quest_list_border->SetVisibility(ESlateVisibility::Visible);
	active_quest_list->SetVisibility(ESlateVisibility::Visible);
	this->Set_QuestDetail(FString(""));
}

void UQuestUI::ArchivedButton()
{
	UE_LOG(LogTemp, Log, TEXT("archived"));
	active_quest_list_border->SetVisibility(ESlateVisibility::Collapsed);
	active_quest_list->SetVisibility(ESlateVisibility::Collapsed);
	archived_quest_list_border->SetVisibility(ESlateVisibility::Visible);
	archived_quest_list->SetVisibility(ESlateVisibility::Visible);
	this->Set_QuestDetail(FString(""));
}

void UQuestUI::Add_ArchivedQuest(UQuestInfo& questinfo)
{
	UQuestContent* quest = Cast<UQuestContent>(active_quest_list->GetChildAt(questinfo.ArrayIndex));
	archived_quest_list->AddChild(quest);
	Remove_ActiveQuest(quest);
}

void UQuestUI::Set_QuestDetail(FString text)
{
	quest_detail->SetText(FText::FromString(text));
	quest_detail->SetFont(default_font);
}

void UQuestUI::Remove_ActiveQuest(UQuestContent* quest)
{
	active_quest_list->RemoveChild(quest);
}

void UQuestUI::RemoveAt_ActiveQuest(int32 quest_index)
{
	active_quest_list->RemoveChildAt(quest_index);
	//quest_list->RemoveChildAt(quest_index);
}

void UQuestUI::ClearUI()
{
	active_quest_list->ClearChildren();
	archived_quest_list->ClearChildren();
}

void UQuestUI::Reconstitution()
{
	//FString title, FString content
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TArray<UQuestInfo*> tempDoingQuestArray = GameInstance->QuestManager->GetDoingQuestList();
	TArray<UQuestInfo*> tempEndQuestArray = GameInstance->QuestManager->GetEndQuestList();

	for (auto iter : tempDoingQuestArray)
	{
		UQuestContent* new_quest = WidgetTree->ConstructWidget<UQuestContent>(UQuestContent::StaticClass());

		/* Becaus of WidgetTree, new_quest members are initialized in here */
		new_quest->list_text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		new_quest->img = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		new_quest->button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

		new_quest->list_text->SetFont(default_font);

		new_quest->AddChild(new_quest->img);
		new_quest->AddChild(new_quest->list_text);
		new_quest->AddChild(new_quest->button);

		/* set the button background color */
		new_quest->button->SetBackgroundColor(FLinearColor(1, 1, 1, 0));

		new_quest->img->SetBrushFromTexture(quest_default_img);

		/* set img anchors */
		UCanvasPanelSlot* canvas_slot = Cast<UCanvasPanelSlot>(new_quest->img->Slot);
		canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		canvas_slot->SetOffsets(FMargin(0));

		/* set button anchors */
		canvas_slot = Cast<UCanvasPanelSlot>(new_quest->button->Slot);
		canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		canvas_slot->SetOffsets(FMargin(0));

		/* set text anchors */
		canvas_slot = Cast<UCanvasPanelSlot>(new_quest->list_text->Slot);
		canvas_slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		canvas_slot->SetPosition(FVector2D(-(text_size_x / 2), -(text_size_y / 2)));
		canvas_slot->SetSize(FVector2D(text_size_x, text_size_y));

		FButtonStyle button_style;

		//slate_brush.
		button_style.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
		button_style.Normal.DrawAs = ESlateBrushDrawType::Image;

		button_style.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
		button_style.Pressed.DrawAs = ESlateBrushDrawType::Image;

		button_style.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
		button_style.Hovered.DrawAs = ESlateBrushDrawType::Image;

		new_quest->button->SetStyle(button_style);
		new_quest->Init(this, iter->Name, iter->Content);
		active_quest_list->AddChild(new_quest);
	}
	
	for (auto iter : tempEndQuestArray)
	{
		UQuestContent* new_quest = WidgetTree->ConstructWidget<UQuestContent>(UQuestContent::StaticClass());

		/* Becaus of WidgetTree, new_quest members are initialized in here */
		new_quest->list_text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		new_quest->img = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		new_quest->button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

		new_quest->list_text->SetFont(default_font);

		new_quest->AddChild(new_quest->img);
		new_quest->AddChild(new_quest->list_text);
		new_quest->AddChild(new_quest->button);

		/* set the button background color */
		new_quest->button->SetBackgroundColor(FLinearColor(1, 1, 1, 0));

		new_quest->img->SetBrushFromTexture(quest_default_img);

		/* set img anchors */
		UCanvasPanelSlot* canvas_slot = Cast<UCanvasPanelSlot>(new_quest->img->Slot);
		canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		canvas_slot->SetOffsets(FMargin(0));

		/* set button anchors */
		canvas_slot = Cast<UCanvasPanelSlot>(new_quest->button->Slot);
		canvas_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		canvas_slot->SetOffsets(FMargin(0));

		/* set text anchors */
		canvas_slot = Cast<UCanvasPanelSlot>(new_quest->list_text->Slot);
		canvas_slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		canvas_slot->SetPosition(FVector2D(-(text_size_x / 2), -(text_size_y / 2)));
		canvas_slot->SetSize(FVector2D(text_size_x, text_size_y));

		FButtonStyle button_style;

		//slate_brush.
		button_style.Normal.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
		button_style.Normal.DrawAs = ESlateBrushDrawType::Image;

		button_style.Pressed.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
		button_style.Pressed.DrawAs = ESlateBrushDrawType::Image;

		button_style.Hovered.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
		button_style.Hovered.DrawAs = ESlateBrushDrawType::Image;

		new_quest->button->SetStyle(button_style);
		new_quest->Init(this, iter->Name, iter->Content);
		archived_quest_list->AddChild(new_quest);
	}
}

/* UQuestContent */
void UQuestContent::Init(UQuestUI* _quest_ui, FString Title, FString detail)
{
	quest_ui = _quest_ui;

	button->OnClicked.AddDynamic(this, &UQuestContent::OnClick);

	/* init list text */
	list_text->SetText(FText::FromString(Title));
	list_text->SetJustification(ETextJustify::Center);
	list_text->Font.Size = list_text_size;

	detail_text = detail;
}

void UQuestContent::OnClick()
{
	quest_ui->Set_QuestDetail(detail_text);
}
