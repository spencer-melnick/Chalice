// Copyright (c) 2020 Spencer Melnick

#include "ComponentVisualizers/InteractionComponentVisualizer.h"
#include "ChaliceGame/Characters/InteractionComponent.h"


// FInteractionComponentVisualizer

void FInteractionComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const FLinearColor Color(0.1f, 0.73f, 0.98f);

	const UInteractionComponent* InteractionComponent = CastChecked<UInteractionComponent>(Component);
	FTransform ComponentTransform = InteractionComponent->GetComponentTransform();
	ComponentTransform.RemoveScaling();
	
	DrawWireSphereCappedCone(PDI, ComponentTransform, InteractionComponent->MaxInteractionDistance, InteractionComponent->InteractionConeAngle,
		32, 8, 10, Color, SDPG_World);
}

