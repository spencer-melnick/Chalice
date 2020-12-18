// Copyright (c) 2020 Spencer Melnick

#include "ChaliceGame/Characters/InteractionComponent.h"
#include "ChaliceGame/Subsystems/InteractionSubsystem.h"
#include "ChaliceGame.h"
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

	// Copy controller rotation if enabled and we have a character owner
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (bUseControllerRotation && Character)
	{
		SetWorldRotation(Character->GetControlRotation());
	}

	// Check that the interaction subsystem exists just in case initialization order didn't happen exactly as expected
	const UWorld* World = GetWorld();
	check(World);
	const UInteractionSubsystem* InteractionSubsystem = World->GetSubsystem<UInteractionSubsystem>();
	if (!InteractionSubsystem)
	{
		return;
	}

	// Track the closest actor by distance squared
	AActor* NearestActor = nullptr;
	float ShortestDistanceSquared = -1.f;
	
	for (AActor* PotentialTarget : InteractionSubsystem->GetActors())
	{
		IInteractiveActor* InteractionInterface = Cast<IInteractiveActor>(PotentialTarget);
		check(InteractionInterface)
		if (!InteractionInterface->CanInteract(GetOwner()))
		{
			continue;
		}

		TOptional<float> TargetDistance = CalculateTargetDistance(InteractionInterface->GetInteractionLocation());
		if (!TargetDistance.IsSet())
		{
			continue;
		}

		if (!NearestActor || TargetDistance.GetValue() < ShortestDistanceSquared)
		{
			NearestActor = PotentialTarget;
			ShortestDistanceSquared = TargetDistance.GetValue();
		}
	}

	// Let SetTarget() handle nullptr, or no target change
	SetTarget(NearestActor);
}


// Interaction controls

void UInteractionComponent::TryInteract()
{
	if (!TargetedActor)
	{
		return;
	}

	IInteractiveActor* InteractionInterface = Cast<IInteractiveActor>(TargetedActor);
	if (!InteractionInterface)
	{
		UE_LOG(LogChaliceGame, Warning, TEXT("%s failed on %s - target %s is not an interactive actor"),
			ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(TargetedActor))
		return;
	}

	InteractionInterface->Interact(GetOwner());
	OnInteract.Broadcast(TargetedActor);
}


// Helpers

void UInteractionComponent::SetTarget(AActor* NewTarget)
{
	if (NewTarget == TargetedActor)
	{
		return;
	}

	if (TargetedActor)
	{
		// Target changed and the old target wasn't null - notify that we untargeted an actor
		IInteractiveActor* InteractionInterface = Cast<IInteractiveActor>(TargetedActor);
		check(InteractionInterface)
		InteractionInterface->OnUntargeted(GetOwner());
		OnUntarget.Broadcast(TargetedActor);
	}

	TargetedActor = NewTarget;

	if (TargetedActor)
	{
		// Target changed and the new target isn't null - notify that we targeted an actor
		IInteractiveActor* InteractionInterface = Cast<IInteractiveActor>(TargetedActor);
		check(InteractionInterface)
		InteractionInterface->OnTargeted(GetOwner());
		OnTarget.Broadcast(TargetedActor);
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
	const FVector TargetDirection = TargetOffset / Distance;
	if (FVector::DotProduct(TargetDirection, GetForwardVector()) > FMath::Cos(FMath::DegreesToRadians(InteractionConeAngle)))
	{
		return TOptional<float>();
	}

	return Distance;
}

