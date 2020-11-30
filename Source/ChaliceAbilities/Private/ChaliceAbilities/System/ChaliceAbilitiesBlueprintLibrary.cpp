// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/System/ChaliceAbilitiesBlueprintLibrary.h"
#include "ChaliceAbilities/System/ChaliceAbilityInterface.h"
#include "ChaliceAbilities/System/ChaliceAbilityComponent.h"


// UChaliceAbilitiesBlueprintLibrary

UChaliceAbilityComponent* UChaliceAbilitiesBlueprintLibrary::GetAbilityComponent(const AActor* Actor)
{
	const IChaliceAbilityInterface* AbilityInterface = Cast<IChaliceAbilityInterface>(Actor);
	if (!AbilityInterface)
	{
		return nullptr;
	}

	return AbilityInterface->GetChaliceAbilityComponent();
}

FGameplayTagContainer UChaliceAbilitiesBlueprintLibrary::GetActorOwnedTags(const AActor* Actor)
{
	FGameplayTagContainer Tags;
	const UChaliceAbilityComponent* AbilityComponent = GetAbilityComponent(Actor);
	if (AbilityComponent)
	{
		AbilityComponent->GetOwnedGameplayTags(Tags);
	}
	return Tags;
}

