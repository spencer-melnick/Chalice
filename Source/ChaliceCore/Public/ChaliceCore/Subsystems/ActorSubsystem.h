// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorSubsystem.generated.h"



/**
 * Subsystem that automatically tracks all actors implementing a specific interface
 */
UCLASS(Abstract)
class CHALICECORE_API UActorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	// Subsystem overrides
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;


	// Actor subsystem interface

	UFUNCTION(BlueprintCallable)
	virtual UClass* GetInterfaceType() const { unimplemented(); return nullptr; }

	UFUNCTION(BlueprintCallable)
	const TArray<AActor*>& GetActors() const { return Actors; }


protected:

	// Helpers

	void RegisterActor(AActor* Actor);
	void UnregisterActor(AActor* Actor);


private:

	// Callbacks

	virtual void WorldBeginPlay();
	virtual void ActorSpawned(AActor* Actor);

	UFUNCTION()
	virtual void ActorEndPlay(AActor* Actor, EEndPlayReason::Type Reason);


	// Private properties

	UPROPERTY()
	TArray<AActor*> Actors;

	bool bWorldBeganPlay = false;
	FDelegateHandle WorldBeginPlayDelegateHandle;
	FOnActorSpawned::FDelegate ActorSpawnedDelegate;
	FDelegateHandle ActorSpawnedDelegateHandle;
	
};
