// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"
#include "ChaliceAbilities/System/ChaliceAbilitySetttings.h"
#include "ChaliceAbilities/Abilities/ChaliceAbility.h"
#include "ChaliceAbilities.h"
#include "GameFramework/Character.h"


// UChaliceAbilityComponent

// Ability system component overrides

void UChaliceAbilityComponent::AbilityLocalInputPressed(int32 InputID)
{
	// Check cancel and confirm inputs
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}
	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	bool bAnyAbilityWantsBuffer = false;
	bool bAbilityActivated = false;

	// Iterate through all ability specs
	ABILITYLIST_SCOPE_LOCK()
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (!Spec.Ability || Spec.InputID != InputID)
		{
			continue;
		}
		Spec.InputPressed = true;

		const UChaliceAbility* ChaliceAbility = Cast<UChaliceAbility>(Spec.Ability);
		if (ChaliceAbility && ChaliceAbility->bBufferInput)
		{
			// Track if any ability bound to this input supports input buffering
			bAnyAbilityWantsBuffer = true;
		}
		
		if (Spec.IsActive())
		{
			// Replicate input events if the spec is already active
			if (Spec.Ability->bReplicateInputDirectly && !IsOwnerActorAuthoritative())
			{
				ServerSetInputPressed(Spec.Handle);
			}

			AbilitySpecInputPressed(Spec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
		}
		else if (TryActivateAbility(Spec.Handle))
		{
			// Try to activate the ability if the spec isn't active
			// Track if any ability activation succeeded
			bAbilityActivated = true;
		}
	}

	if (!bAbilityActivated && bAnyAbilityWantsBuffer)
	{
		// Buffer input if no abilities were activated and a bound input is listening to the buffer
		BufferInput(InputID);
	}
}

void UChaliceAbilityComponent::BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo)
{
	Super::BindAbilityActivationToInputComponent(InputComponent, BindInfo);

	// This might not work if multiple different enums are used for binding
	InputBindEnum = BindInfo.GetBindEnum();
}


bool UChaliceAbilityComponent::GetShouldTick() const
{
	if (UChaliceAbilitySettings::Get()->bInputBufferEnabled && InputBuffer.Num() > 0)
	{
		return true;
	}

	return Super::GetShouldTick();
}

void UChaliceAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PollBufferedInputs();	
}


// Input buffering

void UChaliceAbilityComponent::BufferInput(int32 InputID)
{
	const UChaliceAbilitySettings* AbilitySettings = UChaliceAbilitySettings::Get();
	const UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed on %s - world not found"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	if (!AbilitySettings->bInputBufferEnabled || AbilitySettings->InputBufferSize == 0)
	{
		return;
	}

	while (InputBuffer.Num() >= static_cast<int32>(AbilitySettings->InputBufferSize))
	{
		InputBuffer.RemoveAt(0);
	}

	FBufferedInput NewBufferedInput;
	NewBufferedInput.InputID = InputID;
	NewBufferedInput.BufferedTime = World->GetRealTimeSeconds();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character)
	{
		NewBufferedInput.MovementInput = Character->GetLastMovementInputVector();
	}
	else
	{
		NewBufferedInput.MovementInput = FVector::ZeroVector;
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s could not get movement input on %s - avatar actor %s is not a character"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(GetAvatarActor()))
	}

	InputBuffer.Push(NewBufferedInput);
	UpdateShouldTick();
}

void UChaliceAbilityComponent::PopInput()
{
	if (InputBuffer.Num() > 0)
	{
		InputBuffer.RemoveAt(0);
		UpdateShouldTick();
	}
}

void UChaliceAbilityComponent::PollBufferedInputs()
{
	const UChaliceAbilitySettings* AbilitySettings = UChaliceAbilitySettings::Get();
	const UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed on %s - world not found"),
            ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this))
		return;
	}

	if (!AbilitySettings->bInputBufferEnabled)
	{
		return;
	}

	const float CurrentRealTime = World->GetRealTimeSeconds();
	bUsingBufferedMovementInput = true;

	while (InputBuffer.Num() > 0)
	{
		const FBufferedInput& BufferedInput = InputBuffer[0];
		BufferedMovementInput = BufferedInput.MovementInput;

		if ((CurrentRealTime - BufferedInput.BufferedTime) >= AbilitySettings->InputBufferTime)
		{
			// Pop expired inputs
			PopInput();
			continue;
		}

		bool bAbilityActivated = false;

		ABILITYLIST_SCOPE_LOCK()
		for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			const UChaliceAbility* ChaliceAbility = Cast<UChaliceAbility>(Spec.Ability);

			if (!ChaliceAbility || Spec.InputID != BufferedInput.InputID)
			{
				continue;
			}

			// Simulate input being pressed for ability activation
			const bool bInputAlreadyPressed = Spec.InputPressed;
			Spec.InputPressed = true;
			
			// Try to activate the ability if it isn't already active
			if (!Spec.IsActive() && TryActivateAbility(Spec.Handle))
			{
				// Track if we activated any ability
				bAbilityActivated = true;

				if (!bInputAlreadyPressed)
				{
					// If the input wasn't pressed before our buffered activation, simulate an input release event
					Spec.InputPressed = false;
					if (Spec.Ability && Spec.IsActive())
					{
						if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
						{
							ServerSetInputReleased(Spec.Handle);
						}
						AbilitySpecInputReleased(Spec);
						InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
					}
				}
			}

			// Reset input state
			Spec.InputPressed = bInputAlreadyPressed;
		}

		if (bAbilityActivated)
		{
			// If any ability was triggered, pop the input from the buffer
			PopInput();
			break;
		}
		else
		{
			// If no ability was activated, don't check the next input
			break;
		}
	}

	bUsingBufferedMovementInput = false;
}


// Input controls

FVector UChaliceAbilityComponent::GetLastMovementInput() const
{
	if (bUsingBufferedMovementInput)
	{
		return BufferedMovementInput;
	}

	const ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		UE_LOG(LogChaliceAbilities, Warning, TEXT("%s failed on %s - %s is not a valid character"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(GetAvatarActor()))
		return FVector::ZeroVector;
	}

	return Character->GetLastMovementInputVector();
}

