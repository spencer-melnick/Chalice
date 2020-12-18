// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"


/**
 * Simple component that scans the nearby area for interactive objects within a cone
 */
UCLASS()
class CHALICEGAME_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();
	
	
	// Component overrides
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Editor properties

	UPROPERTY(EditAnywhere, Category="Shape")
	float MaxInteractionDistance;

	UPROPERTY(EditAnywhere, Category="Shape")
	float InteractionConeAngle;

	UPROPERTY(EditAnywhere, Category="Control")
	bool bUseControllerRotation;
};
