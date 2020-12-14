// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "AbilitySubsystem.generated.h"


/**
 * Simple engine subsystem that sets up ability system globals
 */
UCLASS(Config="Game")
class CHALICEABILITIES_API UAbilitySubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:

	// Engine subsystem overrides
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
};
