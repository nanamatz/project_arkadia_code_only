// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/TurnQueueUI/TurnQueuePortraitTooltipStatus.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "Arkadia"

void UTurnQueuePortraitTooltipStatus::SetStatus(UTexture2D& img, FText text)
{
	status_img->SetBrushFromTexture(&img);
	status_text->SetText(FText::Format(LOCTEXT("TurnQueueTooltipStatusText", "{0}"), text));
}

#undef LOCTEXT_NAMESPACE
