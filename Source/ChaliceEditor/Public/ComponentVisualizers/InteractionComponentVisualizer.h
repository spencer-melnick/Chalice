// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"


// Forward declarations

class UInteractionComponent;


/**
 * Renders a cone for the interaction component to show the targeting area
 */
class CHALICEEDITOR_API FInteractionComponentVisualizer : public FComponentVisualizer
{

private:

	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	
};
