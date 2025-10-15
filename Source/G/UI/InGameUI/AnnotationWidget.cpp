// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/InGameUI/AnnotationWidget.h"

#include "Components/TextBlock.h"
#include "G/G.h"

#define LOCTEXT_NAMESPACE "Arkadia"

FReply UAnnotationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	const auto key = InMouseEvent.GetEffectingButton();

	if (key == EKeys::LeftMouseButton)
	{
		SetVisibility(ESlateVisibility::Collapsed);

		return reply;
	}

	return reply;
}

void UAnnotationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*PlayAnimation(fade_in);*/
}

void UAnnotationWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UUserWidget::GetWorld()->GetTimerManager().ClearTimer(timer);
}

void UAnnotationWidget::OnAnnotation(FText text, float end_time, int32 font_size)
{
	UUserWidget::GetWorld()->GetTimerManager().ClearTimer(timer);
	
	FSlateFontInfo font = annotation_text->GetFont();
	font.Size = font_size;
	annotation_text->SetFont(font);
	
	annotation_text->SetText(FText::Format(LOCTEXT("AnnotationText", "{0}"), text));

	if(IsInViewport())
	{
		RemoveFromParent();	
	}

	AddToViewport();
	
	PlayAnimation(fade_in);
	
	GetWorld()->GetTimerManager().SetTimer(timer, [this]()
	{
		PlayAnimation(fade_out);
			GetWorld()->GetTimerManager().SetTimer(timer, [this]() { RemoveFromParent(); },
		                                       fade_out_time, false);
	}, end_time, false);
}

#undef LOCTEXT_NAMESPACE
