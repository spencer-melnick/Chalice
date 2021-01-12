// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Player/ChalicePlayerController.h"
#include "ChaliceGame/Characters/ChaliceCharacter.h"


// AChalicePlayerController

AChalicePlayerController::AChalicePlayerController()
{
	
}


// Ability interface overrides

UAbilitySystemComponent* AChalicePlayerController::GetAbilitySystemComponent() const
{
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetPawn());
	if (!AbilitySystemInterface)
	{
		return nullptr;
	}

	return AbilitySystemInterface->GetAbilitySystemComponent();
}

UChaliceAbilityComponent* AChalicePlayerController::GetChaliceAbilityComponent() const
{
	const IChaliceAbilityInterface* AbilityInterface = Cast<IChaliceAbilityInterface>(GetPawn());
	if (!AbilityInterface)
	{
		return nullptr;
	}

	return AbilityInterface->GetChaliceAbilityComponent();
}


// Accessors

AChaliceCharacter* AChalicePlayerController::GetChaliceCharacter() const
{
	return Cast<AChaliceCharacter>(GetPawn());
}
