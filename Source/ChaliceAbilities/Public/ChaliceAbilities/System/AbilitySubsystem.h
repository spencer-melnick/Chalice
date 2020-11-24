// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "AbilitySubsystem.generated.h"


/**
 * Simple engine subsystem that sets up ability system globals
 */
UCLASS()
class CHALICEABILITIES_API UAbilitySubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
