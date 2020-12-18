// Copyright (c) 2020 Spencer Melnick

#include "ChaliceCore/Subsystems/ActorSubsystem.h"
#include "EngineUtils.h"


// UActorSubsystem

// Subsystem overrides

void UActorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Somewhat slow method for grabbing all the existing actors of this type, but it should be okay if
	// only run on world startup
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (Actor->GetClass()->ImplementsInterface(GetInterfaceType()))
		{
			RegisterActor(Actor);
		}
	}

	ActorSpawnedDelegate.BindUObject(this, &UActorSubsystem::ActorSpawned);
	ActorSpawnedDelegateHandle = GetWorld()->AddOnActorSpawnedHandler(ActorSpawnedDelegate);
}

void UActorSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	for (AActor* Actor : Actors)
	{
		UnregisterActor(Actor);
	}

	GetWorld()->RemoveOnActorSpawnedHandler(ActorSpawnedDelegateHandle);
}



// Helpers

void UActorSubsystem::RegisterActor(AActor* Actor)
{
	Actors.Add(Actor);
	Actor->OnEndPlay.AddUniqueDynamic(this, &UActorSubsystem::ActorEndPlay);
}

void UActorSubsystem::UnregisterActor(AActor* Actor)
{
	Actors.Remove(Actor);

	if (!Actor->IsPendingKill())
	{
		Actor->OnEndPlay.RemoveDynamic(this, &UActorSubsystem::ActorEndPlay);
	}
}


// Callbacks

void UActorSubsystem::ActorSpawned(AActor* Actor)
{
	if (Actor->GetClass()->ImplementsInterface(GetInterfaceType()))
	{
		RegisterActor(Actor);
	}
}

void UActorSubsystem::ActorEndPlay(AActor* Actor, EEndPlayReason::Type Reason)
{
	UnregisterActor(Actor);
}

