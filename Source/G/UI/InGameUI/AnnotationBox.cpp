// Fill out your copyright notice in the Description page of Project Settings.


#include "g/UI/InGameUI/AnnotationBox.h"

#include "AnnotationWidget.h"
#include "G/GGameInstance.h"

AAnnotationBox::AAnnotationBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AAnnotationBox::OnBeginOverlap);
}

void AAnnotationBox::BeginPlay()
{
	Super::BeginPlay();

	this->SetActorEnableCollision(true);
}

void AAnnotationBox::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		this->SetActorEnableCollision(false);
		for (auto annotation_text : annotation_text_arr)
		{
			Cast<UGGameInstance>(GetGameInstance())->GetAnnotationWidget()->OnAnnotation(annotation_text, 3);
		}
	}
}
