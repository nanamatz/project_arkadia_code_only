#include "FXManager.h"

#include "G/G.h"
#include "G/GCharacter.h"
#include "G/Util/Define.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

const FName socketName = TEXT("FX_Head");

UFXManager::UFXManager()
{
	int index = 0;
	const FString VFXPath = FXPath + "VFX/";
	const FString SFXPath = FXPath + "SFX/";
	for (EBUFFTYPE type : TEnumRange<EBUFFTYPE>())
	{
		FString vfxName = UEnum::GetDisplayValueAsText(type).ToString();
		//LOG(Log, "%s", *UEnum::GetDisplayValueAsText(type).ToString());
		FString path = VFXPath + "Buff/" + vfxName + "." + vfxName;

		ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(*path);
		if (ParticleSystem.Succeeded())
		{
			BuffVFXMap.Add(type, ParticleSystem.Object);
		}
		index++;
	}

	index = 0;
	for (EDEBUFFTYPE type : TEnumRange<EDEBUFFTYPE>())
	{
		FString vfxName = UEnum::GetDisplayValueAsText(type).ToString();
		//LOG(Log, "%s", *UEnum::GetDisplayValueAsText(type).ToString());
		FString path = VFXPath + "Debuff/" + vfxName + "." + vfxName;

		ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(*path);
		if (ParticleSystem.Succeeded())
		{
			DebuffVFXMap.Add(type, ParticleSystem.Object);
		}
		index++;
	}

	index = 0;
	for (EVFXType type : TEnumRange<EVFXType>())
	{
		FString vfxName = UEnum::GetDisplayValueAsText(type).ToString();
		//LOG(Log, "%s", *UEnum::GetDisplayValueAsText(type).ToString());
		FString path = VFXPath + "Etc/" + vfxName + "." + vfxName;

		ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(*path);
		if (ParticleSystem.Succeeded())
		{
			VFXMap.Add(type, ParticleSystem.Object);
		}
		index++;
	}

	index = 0;
	for (EHeadVFX type : TEnumRange<EHeadVFX>())
	{
		const FString vfxName = "Head_" + UEnum::GetDisplayValueAsText(type).ToString();
		//LOG(Log, "%s", *UEnum::GetDisplayValueAsText(type).ToString());
		const FString path = VFXPath + "Head/" + vfxName + "." + vfxName;

		ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(*path);
		if (ParticleSystem.Succeeded())
		{
			HeadVFXMap.Add(type, ParticleSystem.Object);
		}
		index++;
	}
}

UParticleSystem* UFXManager::GetVFX(EBUFFTYPE type)
{
	return BuffVFXMap.Contains(type) ? BuffVFXMap[type] : nullptr;
}

UParticleSystem* UFXManager::GetVFX(EDEBUFFTYPE type)
{
	return DebuffVFXMap.Contains(type) ? DebuffVFXMap[type] : nullptr;
}

UParticleSystem* UFXManager::GetVFX(EVFXType type)
{
	return VFXMap.Contains(type) ? VFXMap[type] : nullptr;
}

UParticleSystem* UFXManager::GetVFX(EHeadVFX type)
{
	return HeadVFXMap.Contains(type) ? HeadVFXMap[type] : nullptr;
}

USoundBase* UFXManager::GetSFX(ESFXType type)
{
	return SFXMap.Contains(type) ? SFXMap[type] : nullptr;
}

void UFXManager::AttachHeadVFX(APawn* character, EHeadVFX type)
{
	DeleteHeadVFX(character, type);
	UParticleSystem* particle = GetVFX(type);
	if (particle && character)
	{
		// APawn을 ACharacter로 캐스팅
		ACharacter* Character = Cast<ACharacter>(character);
		if (Character)
		{
			USkeletalMeshComponent* MeshComp = Character->GetMesh();  // 캐릭터의 Skeletal Mesh 가져오기
			if (MeshComp)
			{
				// 소켓에 파티클 붙이기
				UGameplayStatics::SpawnEmitterAttached(particle, MeshComp, socketName);
			}
		}
	}
}


void UFXManager::DeleteHeadVFX(APawn* character, EHeadVFX type)
{
	LOG(Log, "");
	UParticleSystem* particle = GetVFX(type);
	if (particle && character)
	{
		// APawn을 ACharacter로 캐스팅
		ACharacter* Character = Cast<ACharacter>(character);
		if (Character)
		{
			USkeletalMeshComponent* MeshComp = Character->GetMesh();  // 캐릭터의 Skeletal Mesh 가져오기
    
			// 모든 부착된 자식 컴포넌트 확인
			const TArray<USceneComponent*>& AttachedComponents = MeshComp->GetAttachChildren();

			LOG(Log, "Compoent : %d", AttachedComponents.Num());
			for (USceneComponent* Component : AttachedComponents)
			{
				// 파티클 컴포넌트인지 확인
				UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(Component);
				if (ParticleComp && ParticleComp->GetAttachSocketName() == socketName)
				{
					ParticleComp->DestroyComponent();
					break;
				}
			}
			
		}
	}
}


void UFXManager::PlaySFX(ESFXType type, FVector location)
{
	USoundBase* sound = GetSFX(type);
	if (sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GameInstance->GetWorld(), sound, location);
	}
}




