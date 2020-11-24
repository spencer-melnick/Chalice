// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceAbilityInterface.generated.h"


// Forward declarations

class UChaliceAbilityComponent;


/**
 * Simple interface for retrieving the specialized ability system component
 */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UChaliceAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

class CHALICEABILITIES_API IChaliceAbilityInterface
{
	GENERATED_BODY()

	virtual UChaliceAbilityComponent* GetChaliceAbilityComponent() const = 0;
};
