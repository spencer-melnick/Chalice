// Copyright (c) 2020 Spencer Melnick

#include "ChaliceAbilities/System/AbilitySubsystem.h"
#include "ChaliceAbilities.h"
#include "AbilitySystemGlobals.h"


// UAbilitySubsystem

// Engine subsystem overrides

void UAbilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogChaliceAbilities, Display, TEXT("Initializing ability system global data"))
	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UAbilitySubsystem::Deinitialize()
{
	UE_LOG(LogChaliceAbilities, Display, TEXT("Saving configuration values"))
	SaveConfig();
}

