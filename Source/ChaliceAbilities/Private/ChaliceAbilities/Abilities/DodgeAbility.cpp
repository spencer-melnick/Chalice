// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Abilities/DodgeAbility.h"
#include "ChaliceAbilities/Tasks/AT_ApplyRootMotionPositionCurve.h"
#include "ChaliceAbilities.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


// UDodgeAbility

UDodgeAbility::UDodgeAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bBufferInput = true;
}


// Ability overrides

void UDodgeAbility::ActivateAbility(
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
	}

	FVector DodgeDirection = GetLastMovementInput(*ActorInfo);
	DodgeDirection.Z = 0.f;
	DodgeDirection.Normalize();

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("DodgePlayMontage"), DodgeMontage, MontageRate, StartingSection);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_ApplyRootMotionPositionCurve* PositionCurveTask = UAbilityTask_ApplyRootMotionPositionCurve::ApplyRootMotionPositionCurve(
		this, TEXT("DodgePositionCurve"), DodgeDirection, DodgeDistance, DodgeDuration, PositionCurve);
	PositionCurveTask->OnFinish.AddDynamic(this, &UDodgeAbility::DodgeFinished);
	PositionCurveTask->ReadyForActivation();
}

bool UDodgeAbility::CanActivateAbility(
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

	FVector DodgeDirection = GetLastMovementInput(*ActorInfo);
	DodgeDirection.Z = 0.f;
	if (DodgeDirection.IsNearlyZero())
	{
		return false;
	}

	return true;
}


// Callback functions

void UDodgeAbility::DodgeFinished()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

