#include "KilltoRecoverAction.h"

#include "G/CharacterInfoComponent.h"
#include "G/G.h"
#include "G/GCharacter.h"
#include "G/GGameInstance.h"
#include "G/Skill/SkillSystemComponent/SkillSystemComponent.h"
#include "G/UI/InGameUI/InGameUI.h"
#include "Kismet/GameplayStatics.h"

void UKilltoRecoverAction::SkillAction(AGCharacter* myCharacter, USkillBase* skill)
{
	Super::SkillAction(myCharacter, skill);

	LOG(Log, "");
	auto target = myCharacter->GetSkillSystemComponent()->GetSkillTarget();
	if(target && target->GetBDeath())
	{
		myCharacter->GetCharacterInfoManager()->AddCurrentCost(1);
		UGGameInstance* GameInstance = Cast<UGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->GetInGameUI()->SetCost(myCharacter);
	}else
	{
		LOG(Log, "Not Find Target");
	}
}
