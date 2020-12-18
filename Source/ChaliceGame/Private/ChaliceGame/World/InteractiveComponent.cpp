// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/World/InteractiveComponent.h"
#include "ChaliceGame/Subsystems/InteractionSubsystem.h"


// UInteractiveComponent

// Component overrides

void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	UInteractionSubsystem* InteractionSubsystem = UInteractionSubsystem::Get(this);
	if (InteractionSubsystem)
	{
		InteractionSubsystem->RegisterInteractiveComponent(this);
	}
}

void UInteractiveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UInteractionSubsystem* InteractionSubsystem = UInteractionSubsystem::Get(this);
	if (InteractionSubsystem)
	{
		InteractionSubsystem->UnregisterInteractiveComponent(this);
	}
}


// Interaction interface

bool UInteractiveComponent::CanInteract(AActor* InteractionInstigator) const
{
	if (CanInteractDelegate.IsBound())
	{
		bool bResult;
		CanInteractDelegate.Execute(InteractionInstigator, bResult);
		return bResult;
	}

	return true;
}

void UInteractiveComponent::Targeted(AActor* InteractionInstigator)
{
	OnTargetedDelegate.Broadcast(InteractionInstigator);
}

void UInteractiveComponent::Untargeted(AActor* InteractionInstigator)
{
	OnUntargetedDelegate.Broadcast(InteractionInstigator);
}

void UInteractiveComponent::Interact(AActor* InteractionInstigator)
{
	OnInteractDelegate.Broadcast(InteractionInstigator);
}

