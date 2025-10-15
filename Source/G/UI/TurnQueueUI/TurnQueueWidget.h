// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "G/Player/CharacterInfoStruct.h"
#include "Blueprint/UserWidget.h"
#include "TurnQueueWidget.generated.h"

class UHorizontalBox;
class UTexture2D;
class USizeBox;
class UCanvasPanel;
class UTextBlock;

class UTurnQueuePortrait;
class UTurnQueuePortraitTooltip;

struct FDEBUFF;
struct FBUFF;

/**
 * 
 */
UCLASS()
class G_API UTurnQueueWidget : public UUserWidget
{
	GENERATED_BODY()

	const uint8 size_x = 100;
	const uint8 default_trans_y = 10;
	const uint8 portrait_bottom_padding = 0;

	// number of added character
	int8 character_count = 0;
	int current_size_x = 0;
	//int default_trans_y = 25;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* turn_queue_box;

	UPROPERTY(EditAnywhere)
	UTexture2D* portrait_border;

	UPROPERTY(EditAnywhere)
	UTexture2D* portrait_actv;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTurnQueuePortrait> bp_turnqueue_portrait;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTurnQueuePortraitTooltip> bp_turnqueue_tooltip;

	UPROPERTY(meta = (BindWidget))
	USizeBox* tooltip_size_box;

	UPROPERTY()
	UTurnQueuePortraitTooltip* tooltip;

	/* Player Turn Announcement */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* announcement_playerturn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* announcement_text;

	float announcement_on_time = 2;

	FTimerHandle timer;

	UPROPERTY(EditAnywhere)
	TMap<EBUFFTYPE, UTexture2D*> buff_texture_map;

	UPROPERTY(EditAnywhere)
	TMap<EDEBUFFTYPE, UTexture2D*> debuff_texture_map;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundWave* combat_bgm;

	UPROPERTY()
	UAudioComponent* audio_comp;
	
protected:
	virtual void NativeConstruct() override;

private:
	void OffAnnouncement() const;

public:
	void Init();
	void EmphasisPortrait();
	void DeEmphasisPortrait();
	/*void EmphasisPortrait(class UTurnQueuePortrait* portrait);
	void DeEmphasisPortrait(class UTurnQueuePortrait* portrait);*/
	void InsertPortrait(class AGCharacter* character);
	void DeletePortrait(int32 index);
	void FirstToLastPortrait();
	void ClearPortrait();

	void AddBuff(AGCharacter* character, FBUFF* buff);
	void AddDebuff(AGCharacter* character, FDEBUFF* debuff);
	void AddBuff(int32 index, FBUFF* buff);
	void AddDebuff(int32 index, FDEBUFF* debuff);

	void UpdateHP(AGCharacter* character);
	/*void DeleteBuff(int32 index, struct FBUFF* buff);
	void DeleteDebuff(int32 index, struct FDEBUFF* debuff);*/

	void On() { this->AddToViewport(); };
	void Off();

	void AnnouncementPlayerTurn();
	void AnnouncementStartCombat();
	void AnnouncementEndCombat();

	TMap<EBUFFTYPE, UTexture2D*>& GetBuffTextureMap() { return buff_texture_map; }
	TMap<EDEBUFFTYPE, UTexture2D*>& GetDeBuffTextureMap() { return debuff_texture_map; }
};
