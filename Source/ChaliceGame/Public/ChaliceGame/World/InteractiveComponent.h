// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "InteractiveComponent.generated.h"


// Delegate declarations

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCanInteractDelegate, AActor*, InteractionInstigator, bool&, ReturnValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractiveComponentDelegate, AActor*, InteractionInstigator);


/**
 * Component that represents an interactive point in the world, such as a door, switch, item pickup, etc.
 */
UCLASS(Category="Chalice|World", meta=(BlueprintSpawnableComponent))
class CHALICEGAME_API UInteractiveComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	// Component overrides

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	// Interaction interface

	virtual bool CanInteract(AActor* InteractionInstigator) const;
	virtual FVector GetInteractionLocation() const { return GetComponentLocation(); }
	virtual void Targeted(AActor* InteractionInstigator);
	virtual void Untargeted(AActor* InteractionInstigator);
	virtual void Interact(AActor* InteractionInstigator);


	// Delegates

	UPROPERTY(BlueprintReadWrite)
	FCanInteractDelegate CanInteractDelegate;

	UPROPERTY(BlueprintAssignable)
	FInteractiveComponentDelegate OnTargetedDelegate;

	UPROPERTY(BlueprintAssignable)
	FInteractiveComponentDelegate OnUntargetedDelegate;

	UPROPERTY(BlueprintAssignable)
	FInteractiveComponentDelegate OnInteractDelegate;
	
};
