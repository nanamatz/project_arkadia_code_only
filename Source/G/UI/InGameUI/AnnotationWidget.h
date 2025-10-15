// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnnotationWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class G_API UAnnotationWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* annotation_text;

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
	void OnAnnotation(FText text, float end_time, int32 font_size = 20);
};
