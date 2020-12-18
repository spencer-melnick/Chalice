// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractionSubsystem.generated.h"


// Forward declarations

class UInteractiveComponent;


/**
 * Simple subsystem that tracks all interactive components
 */
UCLASS()
class CHALICEGAME_API UInteractionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend UInteractiveComponent;

public:

	// World subsystem overrides

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;


	// Accessors

	static UInteractionSubsystem* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	const TArray<UInteractiveComponent*>& GetInteractiveComponents() const { return InteractiveComponents; }


protected:

	// Tracking controls

	void RegisterInteractiveComponent(UInteractiveComponent* InteractiveComponent);
	void UnregisterInteractiveComponent(UInteractiveComponent* InteractiveComponent);


private:

	UPROPERTY()
	TArray<UInteractiveComponent*> InteractiveComponents;
	
};
