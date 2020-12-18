// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Characters/InteractionComponent.h"
#include "GameFramework/Character.h"


// UInteractionComponent

UInteractionComponent::UInteractionComponent()
{
	// Enable ticking
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


// Component overrides

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (bUseControllerRotation && Character)
	{
		SetWorldRotation(Character->GetControlRotation());
	}
}

