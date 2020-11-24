// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/Abilities/JumpAbility.h"
#include "ChaliceAbilities.h"
#include "GameFramework/Character.h"


// UJumpAbility

UJumpAbility::UJumpAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	bBufferInput = false;
}


// Ability overrides

void UJumpAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		Character->Jump();
	}
}

bool UJumpAbility::CanActivateAbility(
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

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return Character && Character->CanJump();
}

void UJumpAbility::InputReleased(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}

void UJumpAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		// Not entirely sure what this is for - scope locks aren't checked anywhere else, but maybe cancelling is a special case?
		// This is adapted from the example jump ability
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UJumpAbility::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		Character->StopJumping();
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

