// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/TurnQueueUI/TurnQueuePortrait.h"

#include "TurnQueuePortraitTooltip.h"
#include "G/GCharacter.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "G/G.h"
#include "G/GPlayerCamera.h"
#include "G/UI/TurnQueueUI/BuffDebuffImage.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

FReply UTurnQueuePortrait::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetEffectingButton();

	if (key == EKeys::LeftMouseButton)
	{
		target_character->ActiveHighlightInMoment(3);
		tooltip->On(target_character);
		return reply;
	}

	return reply;
}

FReply UTurnQueuePortrait::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                          const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetEffectingButton();


	if (key == EKeys::LeftMouseButton)
	{
		target_character->ActiveHighlightInMoment(3);
		AGPlayerCamera* camera = Cast<AGPlayerCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (camera)
		{
			camera->SetActorLocation(target_character->GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail to get AGPlayerCamera from GetPlayerPawn"));
		}

		return reply;
	}

	return reply;
}

void UTurnQueuePortrait::Init(AGCharacter* character, UTurnQueuePortraitTooltip* _tooltip)
{
	tooltip = _tooltip;
	target_character = character;
	portrait->SetBrushFromTexture(&character->GetPortrait());
	if (character->GetECharacterName() == ECharacterName::NonPlayerCharacter)
	{
		EmphasisEmeny();
	}
	else
	{
		EmphasisCompanion();
	}

	UpdateHP(character->GetCurrentHP(), character->GetMaxHP(), character->GetCurrentShield());
}

void UTurnQueuePortrait::SetBorder(UTexture2D& texture)
{
	portrait_border->SetBrushFromTexture(&texture);
}

void UTurnQueuePortrait::EmphasisCurrTurn()
{
	portrait_border->SetBrushColor(FLinearColor(1, 1, 1));
}

void UTurnQueuePortrait::EmphasisEmeny()
{
	portrait_border->SetBrushColor(FLinearColor(1, 0, 0));
}

void UTurnQueuePortrait::EmphasisCompanion()
{
	portrait_border->SetBrushColor(FLinearColor(0, 0.2f, 1));
}

void UTurnQueuePortrait::DeEmphasis()
{
	if (target_character->GetECharacterName() == ECharacterName::NonPlayerCharacter)
	{
		EmphasisEmeny();
	}
	else
	{
		EmphasisCompanion();
	}
}


void UTurnQueuePortrait::AddBuff(UTexture2D* texture, int32* turn_count)
{
	UBuffDebuffImage* new_image = WidgetTree->ConstructWidget<UBuffDebuffImage>(UBuffDebuffImage::StaticClass());
	new_image->turn_count_ptr = turn_count;
	buff_debuff_arr.Add(new_image);

	new_image->SetBrushSize(FVector2D(10));
	new_image->SetBrushFromTexture(texture);

	int32 child_cnt = buff_debuff_arr.Num() - 1;

	if (child_cnt > 10)
	{
		UE_LOG(LogTemp, Log, TEXT("AddBuff: child_cnt > 10 error, max buff in ui is 10"));

		return;
	}

	if ((child_cnt % max_buff_cnt_in_line) == 0)
	{
		buff_debuff_anchors.Minimum.Y += 1.f / 7.f;
		buff_debuff_anchors.Maximum.Y += 1.f / 7.f;
		buff_debuff_anchors.Minimum.X = 0;
		buff_debuff_anchors.Maximum.X = 0.2f;
		UCanvasPanelSlot* canvas_slot = portrait_canvas->AddChildToCanvas(new_image);

		canvas_slot->SetAnchors(buff_debuff_anchors);
		canvas_slot->SetOffsets(0);
	}
	else
	{
		UCanvasPanelSlot* canvas_slot = portrait_canvas->AddChildToCanvas(new_image);
		buff_debuff_anchors.Minimum.X += 0.2f;
		buff_debuff_anchors.Maximum.X += 0.2f;
		canvas_slot->SetAnchors(buff_debuff_anchors);
		canvas_slot->SetOffsets(0);
	}
}

void UTurnQueuePortrait::AddDebuff(UTexture2D* texture, int32* turn_count)
{
	UBuffDebuffImage* new_image = WidgetTree->ConstructWidget<UBuffDebuffImage>(UBuffDebuffImage::StaticClass());
	new_image->turn_count_ptr = turn_count;
	buff_debuff_arr.Add(new_image);

	new_image->SetBrushSize(FVector2D(10));
	new_image->SetBrushFromTexture(texture);

	int32 child_cnt = buff_debuff_arr.Num() - 1;

	if (child_cnt > 10)
	{
		UE_LOG(LogTemp, Log, TEXT("AddBuff: child_cnt > 10 error, max buff in ui is 10"));

		return;
	}

	if ((child_cnt % max_buff_cnt_in_line) == 0)
	{
		UCanvasPanelSlot* canvas_slot = portrait_canvas->AddChildToCanvas(new_image);
		buff_debuff_anchors.Minimum.Y += 1.f / 7.f;
		buff_debuff_anchors.Maximum.Y += 1.f / 7.f;
		buff_debuff_anchors.Minimum.X = 0;
		buff_debuff_anchors.Maximum.X = 0.2f;

		canvas_slot->SetAnchors(buff_debuff_anchors);
		canvas_slot->SetOffsets(0);
	}
	else
	{
		UCanvasPanelSlot* canvas_slot = portrait_canvas->AddChildToCanvas(new_image);
		buff_debuff_anchors.Minimum.X += 0.2f;
		buff_debuff_anchors.Maximum.X += 0.2f;
		canvas_slot->SetAnchors(buff_debuff_anchors);
		canvas_slot->SetOffsets(0);
	}
}

void UTurnQueuePortrait::UpdateHP(int32 current_hp, int32 max_hp, int32 armor)
{
	status_text->SetText(FText::Format(FText::FromString("<HP>{0}</> / <Armor>{1}</>"), current_hp, armor));
	hp_bar->SetPercent(1 - static_cast<float>(current_hp) / static_cast<float>(max_hp));
}

void UTurnQueuePortrait::ProcessTurn()
{
	for (UWidget* _buff_debuff_img : buff_debuff_arr)
	{
		UBuffDebuffImage* buff_debuff_img = Cast<UBuffDebuffImage>(_buff_debuff_img);
		if(buff_debuff_img->ProcessTurn(buff_debuff_arr, buff_debuff_anchors))
		{
			if(buff_debuff_arr.Num() % 5 == 0)
			{
				buff_debuff_anchors.Minimum.Y -= 1.f/7.f;
				buff_debuff_anchors.Maximum.Y -= 1.f/7.f;

				if(buff_debuff_arr.Num() == 5)
				{
					buff_debuff_anchors.Minimum.X = 0.8f;
					buff_debuff_anchors.Maximum.X = 1.f;
				}
			}
			else
			{
				buff_debuff_anchors.Minimum.X -= 0.2f;
				buff_debuff_anchors.Maximum.X -= 0.2f;
			}
		}
	}
}
