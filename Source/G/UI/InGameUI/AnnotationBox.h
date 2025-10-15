// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "AnnotationBox.generated.h"

/**
 * 
 */

class AActor;

UCLASS()
class G_API AAnnotationBox : public ATriggerBox
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FText> annotation_text_arr;

protected:
	virtual void BeginPlay() override;

public:
	AAnnotationBox();

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
