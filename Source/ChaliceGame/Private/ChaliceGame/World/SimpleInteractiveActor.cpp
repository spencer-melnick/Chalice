// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/World/SimpleInteractiveActor.h"


// ASimpleInteractiveActor

// Interactive actor overrides

void ASimpleInteractiveActor::Interact(AActor* InteractionInstigator)
{
	BP_Interact(InteractionInstigator);
}

