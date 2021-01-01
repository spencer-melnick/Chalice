// Copyright (c) 2020 Spencer Melnick

#include "ChaliceCore/Subsystems/ActorSubsystem.h"
#include "EngineUtils.h"


// UActorSubsystem

// Subsystem overrides

void UActorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	check(World);

	// Bind a world begin play delegate
	WorldBeginPlayDelegateHandle = World->OnWorldBeginPlay.AddUObject(this, &UActorSubsystem::WorldBeginPlay);
	
	// Create and bind an actor spawned delegate
	ActorSpawnedDelegate.BindUObject(this, &UActorSubsystem::ActorSpawned);
	ActorSpawnedDelegateHandle = World->AddOnActorSpawnedHandler(ActorSpawnedDelegate);
}

void UActorSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	for (AActor* Actor : Actors)
	{
		// Untrack actors (though this may not necessarily be required, it couldn't hurt to clean up everything)
		UnregisterActor(Actor);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		// Unbind all delegates
		World->OnWorldBeginPlay.Remove(WorldBeginPlayDelegateHandle);
		World->RemoveOnActorSpawnedHandler(ActorSpawnedDelegateHandle);
	}
}

bool UActorSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
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

void UActorSubsystem::WorldBeginPlay()
{
	bWorldBeganPlay = true;
	
	// Somewhat slow method for grabbing all the existing actors of this type, but it should be okay if
	// only run on world startup
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (Actor->GetClass()->ImplementsInterface(GetInterfaceType()))
		{
			RegisterActor(Actor);
		}
	}	
}

void UActorSubsystem::ActorSpawned(AActor* Actor)
{
	// Don't start tracking actor spawns until the world has actually began play - we're going to need to
	// grab all actors on begin play anyways
	if (bWorldBeganPlay && Actor->GetClass()->ImplementsInterface(GetInterfaceType()))
	{
		RegisterActor(Actor);
	}
}

void UActorSubsystem::ActorEndPlay(AActor* Actor, EEndPlayReason::Type Reason)
{
	UnregisterActor(Actor);
}

