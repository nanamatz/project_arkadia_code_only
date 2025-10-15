// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImageAnnotationWidget.generated.h"

class UImage;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class G_API UImageAnnotationWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* annotation_img;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* fade_in;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* fade_out;

	FTimerHandle timer;

	UPROPERTY(EditAnywhere)
	float fade_out_time = 0.5f;;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void OnAnnotation(UTexture2D* image, float end_time, FAnchors anchors);
};
