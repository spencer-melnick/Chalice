// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/AbilitySubsystem.h"
#include "ChaliceAbilities.h"
#include "AbilitySystemGlobals.h"


// UAbilitySubsystem

void UAbilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UAbilitySystemGlobals::Get().InitGlobalData();

	UE_LOG(LogChaliceAbilities, Display, TEXT("Initialized ability system global data"))
}
