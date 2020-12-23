// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.generated.h"


// Forward declarations

class UInteractionComponent;


/**
 *
 */
UINTERFACE()
class CHALICECORE_API UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * Simple interface that allows access to an interaction component
 */
class CHALICECORE_API IInteractionInterface
{
	GENERATED_BODY()

public:

	virtual UInteractionComponent* GetInteractionComponent() const = 0;
};
