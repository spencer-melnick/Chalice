// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Abilities/AttackAbility.h"
#include "ChaliceAbilities/Tasks/AT_PlayMontageAndWaitForEvent.h"
#include "ChaliceAbilities/System/ChaliceAbilitySetttings.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"


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
	
	UAbilityTask_PlayMontageAndWaitForEvent* MontageTask = UAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this, TEXT("PlayAttackMontage"), AttackMontage, UChaliceAbilitySettings::Get()->HitEventTag.GetSingleTagContainer(), MontageRate, MontageSection);
	MontageTask->EventReceived.AddDynamic(this, &UAttackAbility::OnHitEnemy);
	MontageTask->OnCompleted.AddDynamic(this, &UAttackAbility::OnAnimationEnd);
	MontageTask->OnInterrupted.AddDynamic(this, &UAttackAbility::OnAnimationEnd);
	MontageTask->ReadyForActivation();
}


// Callbacks

void UAttackAbility::OnHitEnemy(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ApplyHitEffect(EventData);
	
	BP_OnHitEnemy(EventTag, EventData);
}

void UAttackAbility::OnAnimationEnd(FGameplayTag EventTag, FGameplayEventData EventData)
{
	BP_OnAnimationEnd();

	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}


// Helpers

void UAttackAbility::ApplyHitEffect(FGameplayEventData EventData)
{
	UChaliceAbilityComponent* AbilityComponent = Cast<UChaliceAbilityComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	if (!CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed to apply hit effect in instance %s - instigator does not have a valid ability system component"),
	        ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	if (!HitEffect)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed to apply hit effect in instance %s - no hit effect class specified"),
	        ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}
	if (!EventData.Target)
	{
		return;
	}
	
	UChaliceAbilityComponent* TargetAbilityComponent = UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(EventData.Target);
	if (!TargetAbilityComponent)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed to apply hit effect in instance %s - target %s does not have an ability system component"),
            ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(EventData.Target))
		return;
	}
	
	const FGameplayEffectContextHandle HitEffectContext = AbilityComponent->MakeEffectContextFromEvent(EventData);
    const FGameplayEffectSpecHandle HitEffectSpec = AbilityComponent->MakeOutgoingSpec(HitEffect, UGameplayEffect::INVALID_LEVEL, HitEffectContext);
	check(HitEffectSpec.Data.IsValid())
	AbilityComponent->ApplyGameplayEffectSpecToTarget(*HitEffectSpec.Data.Get(), TargetAbilityComponent);
}

