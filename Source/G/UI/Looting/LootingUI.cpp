// Fill out your copyright notice in the Description page of Project Settings.


#include "LootingUI.h"
#include "G/Item/ItemBase.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "LootingSlotWidget.h"
#include "G/UI/Inventory/InventoryTooltip.h"
#include "LootingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "G/GGameInstance.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "G/GPlayerController.h"
#include "G/Item/ItemBase.h"
#include "G/NonPlayerCharacter.h"

FReply ULootingUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//UE_LOG(LogTemp, Log, TEXT("LootingUI MouseDown"));
	Cast<AGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->IsMouseOnLootingUI = true;

	return reply;
}

void ULootingUI::init()
{
	inventory_tooltip = CreateWidget<UInventoryTooltip>(this, bp_inventory_tooltip);
	AllGetButton->OnClicked.AddDynamic(this, &ULootingUI::AllGet);
	CloseButton->OnClicked.AddDynamic(this, &ULootingUI::CloseUI);
}

void ULootingUI::SetItemList(class ULootingComponent* _target)
{
	UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (target_LTC != _target)
	{
		target_LTC = _target;

		LootingInventory->ClearChildren();

		for (auto iter : target_LTC->ProvideItemInfoList)
		{
			UItemBase* targetItem = NewObject<UItemBase>(GameInstance, iter.Key.Get());

			UE_LOG(LogTemp, Log, TEXT("ULootingUI.cpp 53 SetitemList - base name : %s, object name : %s"),
				*(iter.Key.GetDefaultObject()->Name.ToString()), *(targetItem->Name.ToString()));

			targetItem->count = iter.Value;
			Insert(targetItem);
		}
	}
}

void ULootingUI::OpenLootingUI(FVector2D position)
{
	FVector2D desiredsize, mousepos, viewportsize, resultpos;
	float scale;
	bool isLEFT, isTOP;

	//mousepos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	viewportsize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	AddToViewport();
	ForceLayoutPrepass();
	desiredsize = GetDesiredSize();

	//UE_LOG(LogTemp, Log, TEXT("LootingUI::OpenLootingUI mouse position : %f, %f"), mousepos.X, mousepos.Y);
	//UE_LOG(LogTemp, Log, TEXT("LootingUI::OpenLootingUI viewport size : %f, %f"), viewportsize.X, viewportsize.Y);
	//UE_LOG(LogTemp, Log, TEXT("LootingUI::OpenLootingUI desired size : %f, %f"), desiredsize.X, desiredsize.Y);
	//UE_LOG(LogTemp, Log, TEXT("LootingUI::OpenLootingUI viewportscale : %f"), scale);

	viewportsize /= scale;
	isLEFT = position.X < (viewportsize.X * DisplayInversionPoint.X);
	isTOP = position.Y < (viewportsize.Y * DisplayInversionPoint.Y);

	resultpos = position;

	resultpos.X += (isLEFT ? 0.0f : -desiredsize.X);
	resultpos.Y += (isTOP ? 0.0f : -desiredsize.Y);

	this->SetPositionInViewport(resultpos, false);

	//UE_LOG(LogTemp, Log, TEXT("LootingUI::OpenLootingUI resultpos : %f, %f"), resultpos.X, resultpos.Y);
}

void ULootingUI::Insert(UItemBase* item)
{
	/* Get last horizontal box */
	UHorizontalBox* last_horizontal_box = Cast<
		UHorizontalBox>(LootingInventory->GetChildAt(LootingInventory->GetChildrenCount() - 1));

	///* if you need to create a new horizontal box, then create one */
	if (!last_horizontal_box || last_horizontal_box->GetChildrenCount() >= max_slot_horizontal)
	{
		UHorizontalBox* new_horizontal_box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

		LootingInventory->AddChild(new_horizontal_box);

		last_horizontal_box = new_horizontal_box;
	}

	/* create slot border */
	ULootingSlotWidget* new_inventory_slot = CreateWidget<ULootingSlotWidget>(GetWorld(), bp_inventory_slot);

	new_inventory_slot->init(item, nullptr, inventory_tooltip, this);
	//item->SetSlotUI(new_inventory_slot);

	new_inventory_slot->icon->SetBrushFromTexture(item->GetIcon());


	/* add slot_border to the child of last_horizontal_box */
	last_horizontal_box->AddChildToHorizontalBox(new_inventory_slot);
}

bool ULootingUI::Delete(int32 idx)
{
	/* get horizontal box in which the target item is placed */
	UHorizontalBox* horizontal_box = Cast<UHorizontalBox>(LootingInventory->GetChildAt(idx / max_slot_horizontal));

	/* if RemoveChild failed, then return false */
	if (!horizontal_box->RemoveChildAt(idx % max_slot_horizontal))
	{
		return false;
	}

	return true;
}

void ULootingUI::LootingInventorySort()
{
	UE_LOG(LogTemp, Log, TEXT("ULootingUI::InventorySort execute"));

	UPROPERTY
	()
	TArray<ULootingSlotWidget*> itemarray;

	int32 Horizencount = LootingInventory->GetChildrenCount();
	for (int j = 0; j < Horizencount; j++)
	{
		UHorizontalBox* iterBox = Cast<UHorizontalBox>(LootingInventory->GetChildAt(j));

		if ((iterBox->GetChildrenCount() != max_slot_horizontal) && (j + 1 < Horizencount))
		{
			UHorizontalBox* nextiterBox = Cast<UHorizontalBox>(LootingInventory->GetChildAt(j + 1));
			iterBox->AddChildToHorizontalBox(nextiterBox->GetChildAt(0));
		}

		if (j + 1 == Horizencount)
		{
			if (iterBox->GetChildrenCount() == 0)
			{
				iterBox->RemoveFromParent();
			}
		}
	}
}

void ULootingUI::InventoryEmptyCheck()
{
	int32 Horizencount = LootingInventory->GetChildrenCount();
	int32 itemCount = 0;

	for (int j = 0; j < Horizencount; j++)
	{
		UHorizontalBox* iterBox = Cast<UHorizontalBox>(LootingInventory->GetChildAt(j));
		itemCount += iterBox->GetChildrenCount();
	}

	if (itemCount == 0)
	{
		ANonPlayerCharacter* ownerchar = Cast<ANonPlayerCharacter>(target_LTC->GetOwner());
		if (ownerchar)
		{
			ownerchar->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	if(itemCount == 0)
		CloseUI();
}

void ULootingUI::AllGet()
{
	UE_LOG(LogTemp, Log, TEXT("ULootingUI::AllGet execute"));
	//int32 horcount = LootingInventory->GetChildrenCount() - 1;

	UPROPERTY
	()
	TArray<ULootingSlotWidget*> itemarray;

	int32 Horizencount = LootingInventory->GetChildrenCount();
	//UE_LOG(LogTemp, Log, TEXT("ULootingUI AllGet : Childrencount : %d"), Horizencount);
	for (int j = 0; j < Horizencount; j++)
	{
		UHorizontalBox* iterBox = Cast<UHorizontalBox>(LootingInventory->GetChildAt(j));

		//UE_LOG(LogTemp, Log, TEXT("ULootingUI AllGet : iterBox Childrencount : %d"), iterBox->GetChildrenCount());
		for (int i = iterBox->GetChildrenCount() - 1; i > -1; i--)
		{
			itemarray.Add(Cast<ULootingSlotWidget>(iterBox->GetChildAt(i)));
		}

		for (int i = itemarray.Num() - 1; i > -1; i--)
		{
			itemarray[i]->AddItem(false);
		}
		itemarray.Empty();
	}
	LootingInventory->ClearChildren();
	target_LTC->ProvideItemInfoList.Empty();

	ANonPlayerCharacter* ownerchar = Cast<ANonPlayerCharacter>(target_LTC->GetOwner());
	if (ownerchar)
	{
		ownerchar->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	CloseUI();
}

void ULootingUI::CloseUI()
{
	UE_LOG(LogTemp, Log, TEXT("ULootingUI::CloseUI execute"));

	AGPlayerController* pgc = Cast<AGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	pgc->LootingUIVariableEditOff();

	RemoveFromViewport();
}
