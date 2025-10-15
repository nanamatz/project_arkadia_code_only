// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define PlayerCharacterAmount 4
#define bp_recording_actor_x 10000000
#define DistanceCursorPosX 0
#define DistanceCursorPosY 30
const FString npcCollisionChannel = "NPC";
const FString playerCollisionChannel = "PLAYER";

const FString MapPath = "/Game/Asset/Maps/";
const FString FXPath = "/Game/Asset/FX/";

constexpr ECollisionChannel NPCChannel = ECC_GameTraceChannel2;
constexpr ECollisionChannel PLAYERChannel = ECC_GameTraceChannel3;
constexpr ECollisionChannel InteractChannel = ECC_GameTraceChannel8;

// DataManager
UENUM()
enum EDataType
{
	PlayerCharacterData,
	NPCData,
	CardenioSkillData,
	FernandoSkillData,
	SamiraSkillData,
	RuthCindaSkillData,
	ConsumableItemSkillData,
	TutorialData,
};

// DataManager
UENUM(BlueprintType)
enum class ECombatNPCName : uint8
{
	BuffGreen            UMETA(DisplayName = "BuffGreen"),
	BuffBlue             UMETA(DisplayName = "BuffBlue"),
	BuffRed              UMETA(DisplayName = "BuffRed"),
	SkeletonKnight       UMETA(DisplayName = "SkeletonKnight"),
	SkeletonArcher       UMETA(DisplayName = "SkeletonArcher"),
	DesertWarrior        UMETA(DisplayName = "DesertWarrior"),
	DesertArcher         UMETA(DisplayName = "DesertArcher"),
	DesertBoss           UMETA(DisplayName = "DesertBoss"),
	DesertSandElemental  UMETA(DisplayName = "DesertSandElemental"),
	Drake                UMETA(DisplayName = "Drake"),
	DustVenom            UMETA(DisplayName = "DustVenom"),
	Guardian             UMETA(DisplayName = "Guardian"),
	KillCrab             UMETA(DisplayName = "KillCrab"),
	VastiaWarrior        UMETA(DisplayName = "VastiaWarrior"),
	VastiaArcher         UMETA(DisplayName = "VastiaArcher"),
	VastiaCaptain        UMETA(DisplayName = "VastiaCaptain"),
	Nicolas              UMETA(DisplayName = "Nicolas"),
};
ENUM_RANGE_BY_FIRST_AND_LAST(ECombatNPCName, ECombatNPCName::BuffGreen, ECombatNPCName::Nicolas)

UENUM(BlueprintType)
enum class EConsumableItemName : uint8
{
	Resurrection UMETA(DisplayName = "Resurrection"),
	Reset UMETA(DisplayName = "Reset"),
	Punishment UMETA(DisplayName = "Punishment"),
	TemporarilyShield UMETA(DisplayName = "TemporarilyShield"),
	Shelter UMETA(DisplayName = "Shelter"),
	Cleanse UMETA(DisplayName = "Cleanse"),
	HPPotion UMETA(DisplayName = "HPPotion"),
	AdvancedPotion UMETA(DisplayName = "AdvancedPotion"),
};

ENUM_RANGE_BY_FIRST_AND_LAST(EConsumableItemName, EConsumableItemName::Resurrection, EConsumableItemName::AdvancedPotion)

// InGameDataManager
UENUM(BlueprintType)
enum class ECharacterName : uint8
{
	Cardenio UMETA(DisplayName = "Cardenio"),
	Fernando UMETA(DisplayName = "Fernando"),
	Samira UMETA(DisplayName = "Samira"),
	RuthCinda UMETA(DisplayName = "RuthCinda"),
	None UMETA(DisplayName = "None"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
};

ENUM_RANGE_BY_FIRST_AND_LAST(ECharacterName, ECharacterName::Cardenio, ECharacterName::RuthCinda)

// CombatManager
UENUM()
enum ECombatState
{
	CombatNone,
	CombatStart,
	BeforeTurnStart,
	PlayerTurn,
	NPCTurn,
	CombatEnd,
};

// PlayerController
UENUM()
namespace EInputState
{
	enum State
	{
		Default,
		Combat,
		UI,
		Dialog,
	};
}

// PlayerCombatController
UENUM()
enum EActionMode
{
	ClickSkill, // 스킬을 골라야 하는 상태, 움직이는 것도 가능
	ClickTarget, // 타겟을 골라야 하는 상태
	BeforeActionSkill, // 스킬 사용 전 상태
	ActionSkill, // 스킬 사용 중인 상태
	Moving, // 이동 중인 상태
	ActionNone, // 자기 턴이 아닐 때 -> 비활성화 상태
};


UENUM()
enum class ESkillType : uint8
{
	TARGETING,
	NONTARGETING,
};

// CHARACTERCIRCLE :	캐릭터 중심으로 원
// MOUSECIRCLE :		마우스 중심 원
// CHARACTERPIE :		캐릭터 중심 부채꼴

UENUM()
enum class ESkillRangeType : uint8
{
	None UMETA(DisplayName = "None"),
	CHARACTERCIRCLE UMETA(DisplayName = "Character Circle"),
	MOUSECIRCLE UMETA(DisplayName = "Mouse Circle"),
	CHARACTERPIE UMETA(DisplayName = "Character Pie")
};

UENUM()
enum class ESkillTargetType : uint8
{
	NONE,
	SELF,
	ALL,
	PLAYER,
	NPC,
	DeadCharacter,
	ALLIgnoreMe,
};

UENUM()
enum class EAttackType
{
	None,
	Default,
	Pierce,
};


UENUM(BlueprintType)
enum class EDEBUFFTYPE : uint8
{
	// 취약
	WEAK UMETA(DisplayName = "WEAK"),
	// 어지럼증
	DAMAGEDECREASE UMETA(DisplayName = "DAMAGEDECREASE"),
	// 저주
	HEALINGDECREASE UMETA(DisplayName = "HEALINGDECREASE"),
	// 기절
	STUN UMETA(DisplayName = "STUN"),
	// 출혈
	APPLYPIERCEDAMAGE UMETA(DisplayName = "APPLYPIERCEDAMAGE"),
};

ENUM_RANGE_BY_FIRST_AND_LAST(EDEBUFFTYPE, EDEBUFFTYPE::WEAK, EDEBUFFTYPE::APPLYPIERCEDAMAGE)

UENUM(BlueprintType)
enum class EBUFFTYPE : uint8
{
	// 총명
	INSIGHTINCREASE UMETA(DisplayName = "INSIGHTINCREASE"),
	// 축복
	COSTINCREASE UMETA(DisplayName = "COSTINCREASE"),
	// 분노
	STRENGTHINCREASE UMETA(DisplayName = "STRENGTHINCREASE"),
	// 가호
	LUCKYINCREASE UMETA(DisplayName = "LUCKYINCREASE"),
	// 기민함
	MOVECOSTINCREASE UMETA(DisplayName = "MOVECOSTINCREASE"),
	// 모든 피해 면역
	INVULNERABILITY UMETA(DisplayName = "INVULNERABILITY"),
	
	// 방어도 증가
	SHIELDINCREASE UMETA(DisplayName = "SHIELDINCREASE"),
	// 회복
	HEALING UMETA(DisplayName = "HEALING"),
	// 코스트 회복
	COSTHEALING UMETA(DisplayName = "COSTHEALING"),
	// 디버프 제거
	DEBUFFCLEANS UMETA(DisplayName = "DEBUFFCLEANS"),
};

ENUM_RANGE_BY_FIRST_AND_LAST(EBUFFTYPE, EBUFFTYPE::INSIGHTINCREASE, EBUFFTYPE::DEBUFFCLEANS)

UENUM(BlueprintType)
enum class EHeadVFX : uint8
{
	Chaos           UMETA(DisplayName = "Chaos"),
	EagleVision     UMETA(DisplayName = "EagleVision"),
	Evil            UMETA(DisplayName = "Evil"),
	ExclamationMark UMETA(DisplayName = "ExclamationMark"),
	Heart           UMETA(DisplayName = "Heart"),
	QuestionMark    UMETA(DisplayName = "QuestionMark"),
	Selection01     UMETA(DisplayName = "Selection_01"),
	Selection02     UMETA(DisplayName = "Selection_02"),
	Silence         UMETA(DisplayName = "Silence"),
	Sleep           UMETA(DisplayName = "Sleep"),
	Stun            UMETA(DisplayName = "Stun"),
	Unlucky         UMETA(DisplayName = "Unlucky"),
};

ENUM_RANGE_BY_FIRST_AND_LAST(EHeadVFX, EHeadVFX::Chaos, EHeadVFX::Unlucky)


UENUM(BlueprintType)
enum class EVFXType : uint8
{
	LevelUp UMETA(DisplayName = "LevelUp"),
	MouseCursor UMETA(DisplayName = "MouseCursor")
};

ENUM_RANGE_BY_FIRST_AND_LAST(EVFXType, EVFXType::LevelUp, EVFXType::MouseCursor)


UENUM(BlueprintType)
enum class ESFXType : uint8
{
	// 레벨 업
	LevelUp UMETA(DisplayName = "LevelUp"),
};

ENUM_RANGE_BY_FIRST_AND_LAST(ESFXType, ESFXType::LevelUp, ESFXType::LevelUp)

UENUM()
enum class EAbility : uint8
{
	Power,
	Armor,
	Strength,
	Insight,
	Swift,
	Relaxed,
	Careful,
	Luck,
};

ENUM_RANGE_BY_FIRST_AND_LAST(EAbility, EAbility::Strength, EAbility::Luck)


UENUM()
enum EDamageType
{
	Default,
	Shield,
	Critical,
	Miss,
};

UENUM()
enum class EGameLevel : uint8
{
	Stage1 UMETA(DisplayName = "Stage1"),
	CaveJail UMETA(DisplayName = "CaveJail"),
	Palace UMETA(DisplayName = "Palace"),
};

UENUM(BlueprintType)
enum class ETutorial : uint8
{
	Camera UMETA(DisplayName = "Camera"),
	Interaction UMETA(DisplayName = "Interaction"),
	InGame UMETA(DisplayName = "InGame"), // 미니맵, 포트레잇, 게임 메뉴 오픈, 이동
	Combat UMETA(DisplayName = "Combat"),
	Inventory UMETA(DisplayName = "Inventory"),
	Skill UMETA(DisplayName = "Skill"),
	Quest UMETA(DisplayName = "Quest"),
	Save UMETA(DisplayName = "Save"),
	Map UMETA(DisplayName = "Map"),
};
