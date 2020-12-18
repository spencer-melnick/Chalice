// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceCore/Subsystems/ActorSubsystem.h"
#include "InteractionSubsystem.generated.h"


/**
 * Interface for actors that can be interacted with by the players, such as doors, switches, item pickups, etc.
 */
UINTERFACE()
class CHALICEGAME_API UInteractiveActor : public UInterface
{
	GENERATED_BODY()
};

class CHALICEGAME_API IInteractiveActor
{
	GENERATED_BODY()

public:

	virtual FVector GetInteractionLocation() const = 0;
	virtual void Interact(AActor* InteractionInstigator) = 0;
};


/**
 * Simple actor subsystem that tracks all interactive actors
 */
UCLASS()
class CHALICEGAME_API UInteractionSubsystem : public UActorSubsystem
{
	GENERATED_BODY()

public:

	// Actor subsystem overrides

	virtual UClass* GetInterfaceType() const override { return UInteractiveActor::StaticClass(); }
	
};
