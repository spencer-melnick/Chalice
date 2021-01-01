// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Abilities/InteractAbility.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"
#include "ChaliceAbilities.h"
#include "ChaliceCore/Components/InteractionComponent.h"


// UInteractAbility

UInteractAbility::UInteractAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bBufferInput = false;
}


// Ability overrides

bool UInteractAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	check(ActorInfo)
	const UInteractionComponent* InteractionComponent = UChaliceAbilitiesBlueprintLibrary::GetInteractionComponent(ActorInfo->AvatarActor.Get());

	if (!InteractionComponent)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("Cannot activate %s : %s has no interaction component"),
			*GetNameSafe(this), *GetNameSafe(ActorInfo->AvatarActor.Get()))
		return false;
	}

	return InteractionComponent->GetTargetedComponent() != nullptr;
}

void UInteractAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}

	if (!CanActivateAbility(Handle, ActorInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	UInteractionComponent* InteractionComponent = UChaliceAbilitiesBlueprintLibrary::GetInteractionComponent(ActorInfo->AvatarActor.Get());
	check(InteractionComponent)
	InteractionComponent->TryInteract();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

