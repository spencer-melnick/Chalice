// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Abilities/ChaliceAbility.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities.h"


// UChaliceAbility

// Input controls

FVector UChaliceAbility::GetLastMovementInput(const FGameplayAbilityActorInfo& ActorInfo) const
{
	const UChaliceAbilityComponent* ChaliceAbilityComponent = Cast<UChaliceAbilityComponent>(ActorInfo.AbilitySystemComponent.Get());
	if (!ChaliceAbilityComponent)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed on %s - ability system component %s is not a valid chalice ability component"),
            ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(ActorInfo.AbilitySystemComponent.Get()))
		return FVector::ZeroVector;
	}

	return ChaliceAbilityComponent->GetLastMovementInput();
}

