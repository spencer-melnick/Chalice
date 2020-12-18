// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ChaliceGame/Subsystems/InteractionSubsystem.h"
#include "GameFramework/Actor.h"
#include "SimpleInteractiveActor.generated.h"


/**
 * Empty actor implementing basic functionality for interactive actors, with Blueprint support
 */
UCLASS()
class ASimpleInteractiveActor : public AActor, public IInteractiveActor
{
	GENERATED_BODY()

public:

	// Interactive actor overrides
	
	virtual FVector GetInteractionLocation() const override { return GetActorLocation(); }
	virtual void Interact(AActor* InteractionInstigator) override;


	// Blueprint events

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Interact"))
	void BP_Interact(AActor* InteractionInstigator);
	
};
