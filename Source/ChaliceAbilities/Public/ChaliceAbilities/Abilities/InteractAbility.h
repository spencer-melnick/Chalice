// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceAbilities/Abilities/ChaliceAbility.h"
#include "InteractAbility.generated.h"


/**
 * Simple ability that allows usage of the interactive and interaction components
 */
UCLASS()
class CHALICEABILITIES_API UInteractAbility : public UChaliceAbility
{
	GENERATED_BODY()
	
public:

	UInteractAbility();


	// Ability overrides

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
};
