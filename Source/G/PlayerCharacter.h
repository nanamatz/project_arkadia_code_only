// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInfoComponent.h"
#include "GGameInstance.h"
#include "G/GCharacter.h"
#include "G/Skill/SkillData/SkillStruct.h"
#include "PlayerCharacter.generated.h"

class AActor;
class UWeaponItemBase;

/**
 * 
 */
UCLASS()
class G_API APlayerCharacter : public AGCharacter
{
	GENERATED_BODY()

protected:
	bool _isControl;

	UPROPERTY(EditAnywhere)
	UMaterial* circular_portrait;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> bp_recording_actor;

	bool _isAbleInteract;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeaponItemBase> bp_bound_weapon;

	UPROPERTY()
	UWeaponItemBase* bound_weapon;
	
public:
	APlayerCharacter();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCharacterCaptureWidget> bp_character_capture_widget;

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* DecalAttackRange;

	UPROPERTY()
	class ASkillRangeActor* SkillRangeActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* DecalPieRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* SelectedCharacterLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* NavPathSplineComponent;

	UPROPERTY(BlueprintReadWrite)
	TArray<class USplineMeshComponent*> PathSplineMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* POICollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNavigationInvokerComponent* NavigationInvokerComponent;

	// 논타겟 스킬 썼을 때, 범위 안에 있는 캐릭터 리스트
	UPROPERTY()
	TArray<AGCharacter*> OverlappedCharacters;

	//클릭 위치
	FVector MousePos;

	// 스킬 범위 표시 타이머(마우스 기준)
	FTimerHandle SkillRangeTimerHandle;

	// 스킬 시전 시간 타이머
	FTimerHandle SkillDelayTimerHandle;

	//상호작용 범위 내의 NPC 목록
	UPROPERTY()
	TArray<UObject*> InteractableinRange;

public:
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	bool GetIsControl() { return _isControl; }
	void SetIsControl(bool bValue);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetDecalAttackRange() { return DecalAttackRange; }
	FORCEINLINE class UDecalComponent* GetDecalPieRange() { return DecalPieRange; }
	FORCEINLINE class USphereComponent* GetInteractionSphere() { return InteractionSphere; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class USplineComponent* GetNavPathSplineComponent() { return NavPathSplineComponent; }

	/*
	* 
	*		전투 처리 (스킬, 사거리 등)
	* 
	*/
	void InitPlayerCharacter();
	
	void OffViewAllSkillRange();
	// 스킬 사용범위 보여주기(캐릭터 기준)
	void OnViewAttackRange();
	// 스킬 사용 범위 보여주기(마우스 기준)
	void ViewSkillRange();

	virtual int32 TakeAttackDamage(int32 DamageNum, bool bPiercingDamage = false) override;
	virtual void Respawn() override;
	virtual void Death() override;

	bool CheckUseSkill(int _chooseSikillSlotNum);
	void SkillAction(AGCharacter* target = nullptr, FVector Pos = FVector::ZeroVector);
	void SkillStart(AGCharacter* target);
	void SkillEnd();

	UFUNCTION()
	void OnPOIColiisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);

	/* 상호작용 인터페이스 */
	// InteractionSphere 들어온 Interactinointerface들 관련 작용	
	UFUNCTION(BlueprintCallable)
	void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                     const FHitResult& SweepResult);
	// InteractionSphere 나가는 Interactinointerface들 관련 작용
	UFUNCTION(BlueprintCallable)
	void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void Interact_Pressed();
	TArray<UObject*> GetInteractableinRange() { return InteractableinRange; }

	void GetSkillTexture();
	// (캐릭터 중심 원, 마우스 중심 원, 캐릭터 중심 부채꼴)
	ESkillRangeType GetSkillRangeType();
	// 스킬 타입 가져오기
	ESkillTargetType GetSelectedSkillTargetType();
	void OnSelectCharacter();
	void OffSelectCharacter();
	UMaterial*& GetCircularPortrait();

	// 이동 경로
	UFUNCTION(BlueprintCallable)
	void ClearPathSplineMeshes();

	UWeaponItemBase* CreateBoundWeapon();
	UWeaponItemBase* GetBoundWeapon();
};
