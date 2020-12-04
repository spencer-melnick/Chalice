// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "ChaliceAbilitySetttings.generated.h"


/**
 * Simple editor accessible settings for the ability system
 */
UCLASS(Config=Game, meta=(DisplayName="Chalice Ability Settings"))
class CHALICEABILITIES_API UChaliceAbilitySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	static const UChaliceAbilitySettings* Get() { return GetDefault<UChaliceAbilitySettings>(); }
	

	// Configuration properties

	UPROPERTY(EditAnywhere, Config)
	bool bInputBufferEnabled = true;

	UPROPERTY(EditAnywhere, Config, meta=(ClampMin="0"))
	float InputBufferTime = 0.5f;

	UPROPERTY(EditAnywhere, Config, meta=(ClampMin="1"))
	uint32 InputBufferSize = 2;

	UPROPERTY(EditAnywhere, Config)
	FGameplayTag HitEventTag;
};
