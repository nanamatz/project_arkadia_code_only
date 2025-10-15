// Fill out your copyright notice in the Description page of Project Settings.


#include "ImageAnnotationWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

FReply UImageAnnotationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UImageAnnotationWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UImageAnnotationWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UUserWidget::GetWorld()->GetTimerManager().ClearTimer(timer);
}

void UImageAnnotationWidget::OnAnnotation(UTexture2D* image, float end_time, FAnchors anchors)
{
	annotation_img->SetBrushFromTexture(image);

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(annotation_img->Slot);
	CanvasPanelSlot->SetAnchors(anchors);
	CanvasPanelSlot->SetOffsets(0);
	
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
