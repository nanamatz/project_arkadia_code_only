#pragma once

#include "CoreMinimal.h"
#include "G/GGameInstance.h"
#include "G/Manager/ManagerBase.h"
#include "G/Util/Define.h"
#include "Kismet/GameplayStatics.h"
#include "FXManager.generated.h"

/**
 * 
 */
UCLASS()
class G_API UFXManager : public UManagerBase
{
	GENERATED_BODY()
	
	UFXManager();

protected:
	
	UPROPERTY(EditAnywhere, Category = "VFX", Meta = (AllowPrivateAccess = true))
	TMap<EBUFFTYPE, class UParticleSystem*> BuffVFXMap;
	
	UPROPERTY(EditAnywhere, Category = "VFX", Meta = (AllowPrivateAccess = true))
	TMap<EDEBUFFTYPE, class UParticleSystem*> DebuffVFXMap;

	UPROPERTY(EditAnywhere, Category = "VFX", Meta = (AllowPrivateAccess = true))
	TMap<EVFXType, class UParticleSystem*> VFXMap;

	UPROPERTY(EditAnywhere, Category = "VFX", Meta = (AllowPrivateAccess = true))
	TMap<EHeadVFX, class UParticleSystem*> HeadVFXMap;
	
	UPROPERTY(EditAnywhere, Category = "SFX", Meta = (AllowPrivateAccess = true))
	TMap<ESFXType, class USoundBase*> SFXMap;
	
public:
	class UParticleSystem* GetVFX(EBUFFTYPE type);
	class UParticleSystem* GetVFX(EDEBUFFTYPE type);
	class UParticleSystem* GetVFX(EVFXType type);
	class UParticleSystem* GetVFX(EHeadVFX type);
	
	class USoundBase* GetSFX(ESFXType type);

	void AttachHeadVFX(class APawn* character, EHeadVFX type);
	void DeleteHeadVFX(class APawn* character, EHeadVFX type);

	template<typename T>
	void PlayVFX(T type, FVector location)
	{
		UParticleSystem* particle = GetVFX(type);
		if (particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GameInstance->GetWorld(), particle, location, FRotator::ZeroRotator, FVector(1), true, EPSCPoolMethod::AutoRelease);
		}
	}

	void PlaySFX(ESFXType type, FVector location);
};

