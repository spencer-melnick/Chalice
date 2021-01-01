// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"


// Forward declarations

class UInteractiveComponent;


// Delegate declarations

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractDelegate, UInteractiveComponent*, InteractiveComponent);
DECLARE_DELEGATE_RetVal(bool, FInteractionComponentCanInteractDelegate);


/**
 * Simple component that scans the nearby area for interactive objects within a cone
 */
UCLASS()
class CHALICECORE_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();
	
	
	// Component overrides
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Interaction controls

	/**
	 * Tries to interact with the currently targeted actor (if any)
	 */
	UFUNCTION(BlueprintCallable)
	void TryInteract();


	// Accessors

	UFUNCTION(BlueprintCallable)
	UInteractiveComponent* GetTargetedComponent() const { return TargetedComponent; }


	// Delegates

	UPROPERTY(BlueprintAssignable)
	FOnInteractDelegate OnInteract;

	UPROPERTY(BlueprintAssignable)
	FOnInteractDelegate OnTarget;

	UPROPERTY(BlueprintAssignable)
	FOnInteractDelegate OnUntarget;

	FInteractionComponentCanInteractDelegate CanInteract;


	// Editor properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shape")
	float MaxInteractionDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shape")
	float InnerInteractionDistance = 50.f;

	// Angle of the cone used for interaction targeting (in degrees)
	UPROPERTY(EditAnywhere, Category="Shape", meta=(ClampMin="0", ClampMax="90"))
	float InteractionConeAngle = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Control")
	bool bUseControllerRotation = true;

	
protected:

	// Helpers

	void UpdateTarget();

	/**
	 * Sets the current targeted actor, broadcasting delegates as necessary
	 * @param NewTarget - New targeted actor, can also be the same as the current targeted actor, or nullptr
	 */
	void SetTarget(UInteractiveComponent* NewTarget);

	/**
	 * Checks if the target location is within the cone of this interaction component
	 * @return Nothing if the target is outside of the targeting cone, distance if it is
	 */
	TOptional<float> CalculateTargetDistance(FVector TargetLocation) const;


private:

	UPROPERTY()
	UInteractiveComponent* TargetedComponent;
	
};
