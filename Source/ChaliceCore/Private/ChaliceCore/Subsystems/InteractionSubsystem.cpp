// Copyright (c) 2020 Spencer Melnick

#include "ChaliceCore/Subsystems/InteractionSubsystem.h"
#include "ChaliceCore/Components/InteractiveComponent.h"


// UInteractionSubsystem

// World subsystem overrides

bool UInteractionSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	switch (WorldType)
	{
		case EWorldType::Game:
		case EWorldType::PIE:
			return true;

		default:
			return false;
	}
}


// Accessors

UInteractionSubsystem* UInteractionSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	return World->GetSubsystem<UInteractionSubsystem>();
}


// Interaction controls

void UInteractionSubsystem::RegisterInteractiveComponent(UInteractiveComponent* InteractiveComponent)
{
	InteractiveComponents.Add(InteractiveComponent);
}

void UInteractionSubsystem::UnregisterInteractiveComponent(UInteractiveComponent* InteractiveComponent)
{
	InteractiveComponents.Remove(InteractiveComponent);
}
