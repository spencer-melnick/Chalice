// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Abilities/AttackAbility.h"
#include "ChaliceAbilities/Tasks/AT_PlayMontageAndWaitForEvent.h"
#include "ChaliceAbilities/System/AbilitySubsystem.h"


// UAttackAbility

UAttackAbility::UAttackAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bBufferInput = true;
}


// Ability overrides

void UAttackAbility::ActivateAbility(
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

	const UAbilitySubsystem* AbilitySubsystem = GEngine->GetEngineSubsystem<UAbilitySubsystem>();
	check(AbilitySubsystem);

	UAbilityTask_PlayMontageAndWaitForEvent* MontageTask = UAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this, TEXT("PlayAttackMontage"), AttackMontage, AbilitySubsystem->HitEventTag.GetSingleTagContainer(), MontageRate, MontageSection);
	MontageTask->EventReceived.AddDynamic(this, &UAttackAbility::OnHitEnemy);
	MontageTask->OnCompleted.AddDynamic(this, &UAttackAbility::OnAnimationEnd);
}


// Callbacks

void UAttackAbility::OnHitEnemy(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Apply a damage effect here!
	
	BP_OnHitEnemy(EventTag, EventData);
}

void UAttackAbility::OnAnimationEnd(FGameplayTag EventTag, FGameplayEventData EventData)
{
	BP_OnAnimationEnd();

	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

