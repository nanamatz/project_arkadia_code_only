// Fill out your copyright notice in the Description page of Project Settings.


#include "G/UI/TurnQueueUI/BuffDebuffImage.h"

#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"

bool UBuffDebuffImage::ProcessTurn(TArray<UBuffDebuffImage*>& buff_debuff_arr, FAnchors& buff_debuff_anchors)
{
	if (turn_count_ptr && *turn_count_ptr == 0)
	{
		/* delete */
		buff_debuff_arr.Remove(this);

		RemoveFromParent();

		return true;
	}

	return false;
}
