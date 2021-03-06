﻿// Copyright (c) 2020 Spencer Melnick

#include "ChaliceCore/Components/InteractionComponent.h"
#include "ChaliceCore/Components/InteractiveComponent.h"
#include "ChaliceCore/Subsystems/InteractionSubsystem.h"
#include "GameFramework/Character.h"


// UInteractionComponent

UInteractionComponent::UInteractionComponent()
{
	// Enable ticking
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// Set defaults
	MaxInteractionDistance = 100.f;
	InteractionConeAngle = 90.f;
	bUseControllerRotation = true;
}


// Component overrides

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Copy controller rotation if enabled and we have a character owner
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (bUseControllerRotation && Character)
	{
		SetWorldRotation(Character->GetControlRotation());
	}

	UpdateTarget();
}


// Interaction controls

void UInteractionComponent::TryInteract()
{
	if (!TargetedComponent)
	{
		return;
	}

	TargetedComponent->Interact(GetOwner());
	OnInteract.Broadcast(TargetedComponent);
}


// Helpers

void UInteractionComponent::UpdateTarget()
{
	if (CanInteract.IsBound() && !CanInteract.Execute())
	{
		// Clear target if we can't interact at all
		SetTarget(nullptr);
		return;
	}
	
	// Check that the interaction subsystem exists just in case initialization order didn't happen exactly as expected
	const UWorld* World = GetWorld();
	check(World);
	const UInteractionSubsystem* InteractionSubsystem = World->GetSubsystem<UInteractionSubsystem>();
	if (!InteractionSubsystem)
	{
		return;
	}

	// Track the closest actor by distanc
	UInteractiveComponent* NearestComponent = nullptr;
	float ShortestDistance = -1.f;
	
	for (UInteractiveComponent* PotentialTarget : InteractionSubsystem->GetInteractiveComponents())
	{
		if (!PotentialTarget->CanInteract(GetOwner()))
		{
			continue;
		}

		TOptional<float> TargetDistance = CalculateTargetDistance(PotentialTarget->GetInteractionLocation());
		if (!TargetDistance.IsSet())
		{
			continue;
		}

		if (!NearestComponent || TargetDistance.GetValue() < ShortestDistance)
		{
			NearestComponent = PotentialTarget;
			ShortestDistance = TargetDistance.GetValue();
		}
	}

	// Let SetTarget() handle nullptr, or no target change
	SetTarget(NearestComponent);
}


void UInteractionComponent::SetTarget(UInteractiveComponent* NewTarget)
{
	if (NewTarget == TargetedComponent)
	{
		return;
	}

	if (TargetedComponent)
	{
		// Target changed and the old target wasn't null - notify that we untargeted a component
		TargetedComponent->Untargeted(GetOwner());
		OnUntarget.Broadcast(TargetedComponent);
	}

	TargetedComponent = NewTarget;

	if (TargetedComponent)
	{
		// Target changed and the new target isn't null - notify that we targeted a component
		TargetedComponent->Targeted(GetOwner());
		OnTarget.Broadcast(TargetedComponent);
	}
}

TOptional<float> UInteractionComponent::CalculateTargetDistance(FVector TargetLocation) const
{
	const FVector TargetOffset = TargetLocation - GetComponentLocation();
	const float DistanceSquared = TargetOffset.SizeSquared();

	if (DistanceSquared > (MaxInteractionDistance * MaxInteractionDistance))
	{
		return TOptional<float>();
	}

	const float Distance = FMath::Sqrt(DistanceSquared);

	if (Distance > (InnerInteractionDistance * InnerInteractionDistance))
	{
		// Compute angle if the target is outside of the inner sphere radius
		const FVector TargetDirection = TargetOffset / Distance;
		const float TargetAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetDirection, GetForwardVector())));
		if (TargetAngle > InteractionConeAngle)
		{
			return TOptional<float>();
		}
	}

	return Distance;
}

